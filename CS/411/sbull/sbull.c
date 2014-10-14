/*
 * Modified version of sbull.c 
 */
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>		/* everything... */
#include <linux/errno.h>	/* error codes */
#include <linux/timer.h>
#include <linux/types.h>	/* size_t */
#include <linux/fcntl.h>	/* O_ACCMODE */
#include <linux/hdreg.h>	/* HDIO_GETGEO */
#include <linux/kdev_t.h>
#include <linux/vmalloc.h>
#include <linux/genhd.h>
#include <linux/blkdev.h>
#include <linux/buffer_head.h>	/* invalidate_bdev */
#include <linux/bio.h>
#include <linux/crypto.h>

MODULE_LICENSE("Dual BSD/GPL");

/* 
 * Global variables
 */
static int sbull_major = 0;
module_param(sbull_major, int, 0);
static int hardsect_size = 512;
module_param(hardsect_size, int, 0);
static int nsectors = 1024;
module_param(nsectors, int, 0);
static int ndevices = 4;
module_param(ndevices, int, 0);

/* support for the key to be set as a module param */
static char *key = "cs411group6";
module_param(key, charp, 0000);

#define ENCRYPT 1
#define DECRYPT 0

/* encryption method used for the driver and initialized in  sbull_init*/
#define ENCRYPTION_METHOD "aes" 

struct crypto_cipher *cipher;

/*
* The different "request modes" we can use.
*/
enum {
	RM_SIMPLE = 0,		/* The extra-simple request function */
	RM_FULL = 1,		/* The full-blown version */
	RM_NOQUEUE = 2,		/* Use make_request */
};
static int request_mode = RM_SIMPLE;
module_param(request_mode, int, 0);

/*
* Minor number and partition management.
*/
#define SBULL_MINORS 16
#define MINOR_SHIFT 4
#define DEVNUM(kdevnum) (MINOR(kdev_t_to_nr(kdevnum)) >> MINOR_SHIFT

/*
* We can tweak our hardware sector size, but the kernel talks to us
* in terms of small sectors, always.
*/
#define KERNEL_SECTOR_SIZE 512

/*
* After this much idle time, the driver will simulate a media change.
*/
#define INVALIDATE_DELAY 30*HZ

/*
* The internal representation of our device.
*/
struct sbull_dev {
	int size;		/* Device size in sectors */
	u8 *data;		/* The data array */
	short users;		/* How many users */
	short media_change;	/* Flag a media change? */
	spinlock_t lock;	/* For mutual exclusion */
	struct request_queue *queue;	/* The device request queue */
	struct gendisk *gd;	/* The gendisk structure */
	struct timer_list timer;	/* For simulated media changes */
};
static struct sbull_dev *Devices = NULL;

/*
 * Prints the memory to /var/log/messages along with
 * a header that indicates whether or not the data being
 * printed is encrypted or not.
 */
static void log_memory(char *buf, int len, int enc)
{
  int i;

  /* Print the appropriate header */
  if (enc) 
    printk(KERN_INFO "-------- ENCRYPTED DATA --------");
  else
    printk(KERN_INFO "-------- UNENCRYPTED DATA --------");
  
  printk(KERN_INFO "### HEX ###\n");

  /* Print the data */
  for (i = 0; i < len; i++) {
    if (i%20 == 0)
      printk("\n");

    printk("%02x ", buf[i]);
  }
  
  printk("\n\n");

  printk(KERN_INFO "### CHAR ###\n");
  for (i = 0; i < len; i++) {
    printk("%c", buf[i]);
  }

  printk("\n\n");
}

/*
 * Encrypt and write input buffer to disk
 * 
 * The for loop increments by the encryption block size encrypting small
 * sections of each block at a time and sending the output of the encryption
 * directly to the location on disk by giving it the pointer of that location.
 * memset is just to make sure there is no strange addative behavior with 
 * previous memory.
 */
static void sbull_write_disk(char* buffer, unsigned long offset, 
			unsigned long nbytes, int step, u8 *disk) 
{
	int i;
	printk(KERN_INFO "RAMDISK WRITE");
	log_memory(buffer, nbytes, DECRYPT);

	/* encrypt data in chunks of the cipher's block size */
	for (i = 0; i < nbytes; i += step) {
	  	/* zero out the region for encryption */
		memset(disk+offset+i, 0, step);
		/* encrypt the next region */
		crypto_cipher_encrypt_one(cipher, disk+offset+i, buffer+i);
	}
	
	log_memory(disk+offset, nbytes, ENCRYPT);
}

/*
 * Read and decrypt disk
 * 
 * The for loop increments by the encryption block size reading small
 * sections of disk block at a time and sending the output of the dencryption
 * into the output buffer.
 */
static void sbull_read_disk(char* buffer, unsigned long offset,
			unsigned long nbytes, int step, u8 *disk) 
{
  	int i;
	printk(KERN_INFO "RAMDISK READ");
	log_memory(disk+offset, nbytes, ENCRYPT);

	/* decrypt data in chunks of the cipher's block size */
	for (i = 0; i < nbytes; i += step)
		crypto_cipher_decrypt_one(cipher, buffer+i, disk+offset+i);

	log_memory(buffer, nbytes, DECRYPT);
}

/*
 * Handle an I/O request
 *
 * Added the encryption to the orginal sbull algorithm.  The
 * original function wrote/read the data all at once whereas this
 * modified version must write/read the data in increments of the
 * crypto block size.
 */
static void
sbull_transfer(struct sbull_dev *dev, unsigned long sector,
	       unsigned long nsect, char *buffer, int write)
{
	unsigned long offset = sector*KERNEL_SECTOR_SIZE;
	unsigned long nbytes = nsect*KERNEL_SECTOR_SIZE;
	u8 *data = dev->data;

	int step;
	/* something is wrong so stop here if this is true */
	if ((offset+nbytes) > dev->size) {
		printk(KERN_NOTICE "Beyond-end write (%ld %ld)\n", offset,
		       nbytes);
		return;
	}

	/* set our loop step size for using the cipher */
	step = crypto_cipher_blocksize(cipher);
	if (write)
		sbull_write_disk(buffer, offset, nbytes, step, data);
	else 
		sbull_read_disk(buffer,offset, nbytes, step, data);
}

/*
 * Taken from SBULL
 *
* The simple form of the request function.
*/
static void
sbull_request(struct request_queue *q)
{
	struct request *req;

	req = blk_fetch_request(q);
	while (req != NULL) {
		struct sbull_dev *dev = req->rq_disk->private_data;
		if (req->cmd_type != REQ_TYPE_FS) {
			printk(KERN_NOTICE "Skip non-fs request\n");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		sbull_transfer(dev, blk_rq_pos(req),
			       blk_rq_cur_sectors(req), req->buffer,
			       rq_data_dir(req));
		/* end_request(req, 1); */
		if (!__blk_end_request_cur(req, 0)) {
			req = blk_fetch_request(q);
		}
	}
}

/*
 * Taken from SBULL
 *
 * Transfer a single BIO. 
 * Iterate over every page/segment in the BIO struct and transfer its contents
 * The contents are stored in a temporary buffer that is mapped and then 
 * later unmapped.
*/
static int
sbull_xfer_bio(struct sbull_dev *dev, struct bio *bio)
{
	int i;
	struct bio_vec *bvec;
	sector_t sector = bio->bi_sector;

	/* Do each segment independently. */
	bio_for_each_segment(bvec, bio, i) {
		char *buffer = __bio_kmap_atomic(bio, i, KM_USER0);
		sbull_transfer(dev, sector, bio_cur_bytes(bio) >> 9	/* in sectors */
			       , buffer, bio_data_dir(bio) == WRITE);
		sector += bio_cur_bytes(bio) >> 9;	/* in sectors */
		__bio_kunmap_atomic(bio, KM_USER0);
	}
	return 0;		/* Always "succeed" */
}

/*
 * Taken from SBULL
 *
 * Transfer a full request.
 * 
 * Iterate over each bio in an IO request and transfer that bio
 * return the number of sectors in that request.
 */
static int
sbull_xfer_request(struct sbull_dev *dev, struct request *req)
{
	struct bio *bio;
	int nsect = 0;

	__rq_for_each_bio(bio, req) {
		sbull_xfer_bio(dev, bio); /*transfer bio contents */
		nsect += bio->bi_size / KERNEL_SECTOR_SIZE;
	}
	return nsect;
}

/* KATHERINE
 * Taken from SBULL
 * KATHERINE IS COMMENTING THIS
 * Smarter request function that "handles clustering".
*/
static void
sbull_full_request(struct request_queue *q)
{
	struct request *req;
	int sectors_xferred;
	struct sbull_dev *dev = q->queuedata;

	req = blk_fetch_request(q);
	while (req != NULL) {
		if (req->cmd_type != REQ_TYPE_FS) {
			printk(KERN_NOTICE "Skip non-fs request\n");
			__blk_end_request_all(req, -EIO);
			continue;
		}
		sectors_xferred = sbull_xfer_request(dev, req);
		if (!__blk_end_request_cur(req, 0)) {
			blk_fetch_request(q);
		}
	}
}

/* KATHERINE
 * Taken from SBULL
 * KATHERINE IS COMMENTING THIS
 * The direct make request version.
*/
static int
sbull_make_request(struct request_queue *q, struct bio *bio)
{
	struct sbull_dev *dev = q->queuedata;
	int status;

	status = sbull_xfer_bio(dev, bio);
	bio_endio(bio, status);
	return 0;
}

/* 
 * Taken from SBULL
 * 
 * Add drive to system and increment user number.
 * This and sbull_release simulate a removable drive.
*/
static int
sbull_open(struct block_device *device, fmode_t mode)
{
	struct sbull_dev *dev = device->bd_disk->private_data;
	/* kill timer associated with this device: it may be active */
	del_timer_sync(&dev->timer);
	/* filp->private_data = dev; */
	spin_lock(&dev->lock);
	/* revalidate on first open */
	if (!dev->users)
		check_disk_change(device);
	/* increment user number */
	dev->users++;
	/* unlock drive so other processes may access */
	spin_unlock(&dev->lock);
	return 0;
}

/* 
 * Taken from SBULL
 * 
 * Release user from Drive. Disk is ejected when number of users is 0.
 */
static int
sbull_release(struct gendisk *disk, fmode_t mode)
{
	struct sbull_dev *dev = disk->private_data;
	/* lock drive so other processes may not access while in use */
	spin_lock(&dev->lock);
	/* decrement user number */
	dev->users--;
	/*
     * If the device is closed for the last time, start a timer
     * to release RAM in half a minute. The function and argument
     * for the timer have been setup in sbull_init()
    */
	if (!dev->users) {
		dev->timer.expires = jiffies + INVALIDATE_DELAY;
		add_timer(&dev->timer);
	}
	/* unlock drive so other processes may access */
	spin_unlock(&dev->lock);

	return 0;
}

/*
 * Taken from SBULL
 *
 * Look for a (simulated) media change.
*/
int
sbull_media_changed(struct gendisk *gd)
{
	struct sbull_dev *dev = gd->private_data;

	return dev->media_change;
}

/*
 * Taken from SBULL
 *
* Revalidate.  WE DO NOT TAKE THE LOCK HERE, for fear of deadlocking
* with open.  That needs to be reevaluated.
* This function is called after a media change.
*/
int
sbull_revalidate(struct gendisk *gd)
{
	struct sbull_dev *dev = gd->private_data;

	if (dev->media_change) {
		dev->media_change = 0;
		memset(dev->data, 0, dev->size);
	}
	return 0;
}

/*
 * Taken from SBULL
 *
* The "invalidate" function runs out of the device timer; it sets
* a flag to simulate the removal of the media.
*/
void
sbull_invalidate(unsigned long ldev)
{
	struct sbull_dev *dev = (struct sbull_dev *) ldev;

	spin_lock(&dev->lock);
	if (dev->users || !dev->data)
		printk(KERN_WARNING "sbull: timer sanity check failed\n");
	else
		dev->media_change = 1;
	spin_unlock(&dev->lock);
}

/* 
 * getgeo() function that is derived from the original 
 * ioctl function in the sbull source code.  In newer kernels
 * the block layer intercepts the ioctl command and calls
 * the getgeo() function in the corresponding block device
 * driver 
 * 
 * To be honest we have no idea why these needs to be set the way. We haven't
 * covered the hd_geometry struct and frankly seems beyond the scope
 * of this project.
 */
static int
sbull_getgeo(struct block_device *device, struct hd_geometry *geo)
{
	struct sbull_dev *dev = device->bd_disk->private_data;
	long size = dev->size * (hardsect_size / KERNEL_SECTOR_SIZE);
	geo->cylinders = (size & ~0x3f) >> 6;
	geo->heads = 4;
	geo->sectors = 16;

	// TODO: check this value
	geo->start = 4;
	return 0;
}

/*
 * The device operations structure.
 *
 * These are function pointers that define the interface with the kernel
 */
static struct block_device_operations sbull_ops = {
	.owner = THIS_MODULE,
	.open = sbull_open,
	.release = sbull_release,
	.media_changed = sbull_media_changed,
	.revalidate_disk = sbull_revalidate,

	/* the getgeo() definition for ioctl */
	.getgeo = sbull_getgeo
};

/*
 * Taken from SBULL
 *
* Set up our internal device.
* Called by the init function.
*/
static void
setup_device(struct sbull_dev *dev, int which)
{
	/*
	* Get some memory.
	*/
	memset(dev, 0, sizeof (struct sbull_dev));
	dev->size = nsectors * hardsect_size; 
	dev->data = vmalloc(dev->size); /* allocate memory*/
	if (dev->data == NULL) {
		printk(KERN_NOTICE "vmalloc failure.\n");
		return;
	}
	/* Allocate a spinlock for mutual exclusion */
	spin_lock_init(&dev->lock);

	/*
	 * The timer which "invalidates" the device.
	 * This is a 30-second timer used to simulate 
	 * behavior of a removable device.
	 */
	init_timer(&dev->timer);
	dev->timer.data = (unsigned long) dev;
	dev->timer.function = sbull_invalidate;

	/*
	 * The I/O queue, depending on whether we are using our own
	 * make_request function or not.
	 */
	switch (request_mode) {
	case RM_NOQUEUE:
		dev->queue = blk_alloc_queue(GFP_KERNEL);
		if (dev->queue == NULL)
			goto out_vfree;
		blk_queue_make_request(dev->queue, sbull_make_request);
		break;
	case RM_FULL:
		/* blk_init_queue() allocates the request queue. */
		dev->queue = blk_init_queue(sbull_full_request, &dev->lock);
		if (dev->queue == NULL)
			goto out_vfree;
		break;
	default:
		printk(KERN_NOTICE
		       "Bad request mode %d, using simple\n", request_mode);
		/* fall into.. */
	case RM_SIMPLE:
		dev->queue = blk_init_queue(sbull_request, &dev->lock);
		if (dev->queue == NULL)
			goto out_vfree;
		break;
	}
	blk_queue_logical_block_size(dev->queue, hardsect_size);
	dev->queue->queuedata = dev;

	/*
	 * And the gendisk structure.
	 * gendisk is the kernel's representation of an individual disk device.
	 */
	dev->gd = alloc_disk(SBULL_MINORS);
	if (!dev->gd) {
		printk(KERN_NOTICE "alloc_disk failure\n");
		goto out_vfree;
	}
	dev->gd->major = sbull_major;
	dev->gd->first_minor = which * SBULL_MINORS;
	dev->gd->fops = &sbull_ops;
	dev->gd->queue = dev->queue;
	dev->gd->private_data = dev;
	/* 
	 * Set the device names to sbulla, sbullb, etc.
	 */
	snprintf(dev->gd->disk_name, 32, "sbull%c", which + 'a');
	set_capacity(dev->gd, nsectors * (hardsect_size / KERNEL_SECTOR_SIZE));
	add_disk(dev->gd);
	return;

      out_vfree:
	if (dev->data)
		vfree(dev->data);
}

/* 
 * Module initialization function.
 * 
 * Added the cipher initialization code to the default
 * initialization routine for the sbull
 */
static int __init
sbull_init(void)
{
	int i;
	/* init the cipher for later use */
	cipher = crypto_alloc_cipher(ENCRYPTION_METHOD, 4, CRYPTO_ALG_ASYNC);

	/* error out if the cipher init failed */
	if (unlikely(IS_ERR(cipher))) {
		printk(KERN_ERR "sbull: failed to alloc the cipher\n");

		/* 
		 * return the error number retrieved from the pointer
		 * generated by the IS_ERR macro
                 */
		return PTR_ERR(cipher);
	}

	/* set the key appropriately */
	crypto_cipher_setkey(cipher, key, strlen(key));

	/*
	* Get registered.
	* Register a block device called "sbull."
	*/
	sbull_major = register_blkdev(sbull_major, "sbull");
	if (sbull_major <= 0) {
		printk(KERN_WARNING "sbull: unable to get major number\n");
		return -EBUSY;
	}

	/*
	* Allocate the device array, and initialize each one.
	*/
	Devices = kmalloc(ndevices * sizeof (struct sbull_dev), GFP_KERNEL);
	if (Devices == NULL)
		goto out_unregister;
	/* Setup each device */
	for (i = 0; i < ndevices; i++)
		setup_device(Devices + i, i);

	return 0;
      out_unregister:
		unregister_blkdev(sbull_major, "sbull");
		return -ENOMEM;
}

/* 
 * Module exit function.
 * 
 * Added the code to free the memory used by the
 * cipher back to the kernel.
 */
static void
sbull_exit(void)
{
	int i;
	for (i = 0; i < ndevices; i++) {
		struct sbull_dev *dev = Devices + i;

		del_timer_sync(&dev->timer);
		if (dev->gd) {
			del_gendisk(dev->gd);
			put_disk(dev->gd);
		}
		if (dev->queue)
			/* Destroy the request queue by releasing the request_queue_t */
			blk_cleanup_queue(dev->queue);
		if (dev->data)
			/* Free the virtual memory allocated earlier for the disk */
			vfree(dev->data);
	}
	/* Unregister the sbull block device */
	unregister_blkdev(sbull_major, "sbull");

	/* free the resources used by the cipher */
	crypto_free_cipher(cipher);
	kfree(Devices);
}

module_init(sbull_init);
module_exit(sbull_exit);
