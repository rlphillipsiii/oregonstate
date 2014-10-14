/*
 * elevator sstf
 */
#include <linux/blkdev.h>
#include <linux/elevator.h>
#include <linux/bio.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/init.h>

#define SSTF_UP   0
#define SSTF_DOWN 1

struct sstf_data {
	struct list_head queue;
  sector_t current_pos;
  int direction;
};

static struct request *
sstf_latter_request(struct request_queue *q, struct request *rq);

static struct request *
sstf_former_request(struct request_queue *q, struct request *rq);

static void sstf_merged_requests(struct request_queue *q, struct request *rq,
				 struct request *next)
{
	list_del_init(&next->queuelist);
}

static void log_queue(struct request_queue *q)
{
  struct request *loop;
  struct sstf_data *nd = q->elevator->elevator_data;

  printk(KERN_INFO "------ QUEUE ------\n");

  list_for_each_entry(loop, &nd->queue, queuelist) {
    printk(KERN_INFO "%d ", (int) loop->bio->bi_sector);
  }

  printk(KERN_INFO "\n-------------------\n\n");
}

static int sstf_dispatch(struct request_queue *q, int force)
{
  struct sstf_data *nd = q->elevator->elevator_data;

	if (!list_empty(&nd->queue)) {
		struct request *rq;
		rq = list_entry(nd->queue.next, struct request, queuelist);
		list_del_init(&rq->queuelist);
		elv_dispatch_sort(q, rq);

		/* check for a change of direction */
		if (rq->bio->bi_sector > nd->current_pos)
		  nd->direction = SSTF_UP;
		else
		  nd->direction = SSTF_DOWN;

		/* update the current position of the head */
		nd->current_pos = rq->bio->bi_sector;

		log_queue(q);
		return 1;
	}
	return 0;
}

static void sstf_add_request_forward(struct request_queue *q,
				     struct request *rq)
{
  struct request *loop;
  struct request *next;

  struct sstf_data *nd = q->elevator->elevator_data;

  if (nd->current_pos < rq->bio->bi_sector) {
    list_for_each_entry(loop, &nd->queue, queuelist) {
      next = sstf_latter_request(q, rq);

      /* we're at the end of the list in the current direction */
      if (next == NULL || loop->bio->bi_sector > next->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
	return;
      }

      /* we found the spot to place the new request */
      if (loop->bio->bi_sector < rq->bio->bi_sector && 
	  next->bio->bi_sector > rq->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
      }
    }
  } else {
    list_for_each_entry(loop, &nd->queue, queuelist) {
      next = sstf_latter_request(q, rq);

      if (next == NULL || loop->bio->bi_sector < next->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
	return;
      }

      if (loop->bio->bi_sector > rq->bio->bi_sector && 
	  next->bio->bi_sector < rq->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
      }
    }
  }
}

static void sstf_add_request_backward(struct request_queue *q,
				     struct request *rq)
{
  struct request *loop;
  struct request *next;

  struct sstf_data *nd = q->elevator->elevator_data;

  if (nd->current_pos > rq->bio->bi_sector) {
    list_for_each_entry(loop, &nd->queue, queuelist) {
      next = sstf_latter_request(q, rq);

      /* we're at the end of the list in the current direction */
      if (next == NULL || loop->bio->bi_sector < next->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
	return;
      }

      /* we found the spot to place the new request */
      if (loop->bio->bi_sector > rq->bio->bi_sector && 
	  next->bio->bi_sector < rq->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
      }
    }
  } else {
    list_for_each_entry(loop, &nd->queue, queuelist) {
      next = sstf_latter_request(q, rq);

      if (next == NULL || loop->bio->bi_sector > next->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
	return;
      }

      if (loop->bio->bi_sector < rq->bio->bi_sector && 
	  next->bio->bi_sector > rq->bio->bi_sector) {
	list_add(&rq->queuelist, &loop->queuelist);
      }
    }
  }
}

static void sstf_add_request(struct request_queue *q, struct request *rq)
{
  struct sstf_data *nd = q->elevator->elevator_data;

	if (list_empty(&nd->queue)) {
	  list_add_tail(&rq->queuelist, &nd->queue);
	  return;
	}

	if (nd->direction == SSTF_UP)
	  sstf_add_request_forward(q, rq);
	else if (nd->direction == SSTF_DOWN)
	  sstf_add_request_backward(q, rq);	
}

static struct request *
sstf_former_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.prev == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.prev, struct request, queuelist);
}

static struct request *
sstf_latter_request(struct request_queue *q, struct request *rq)
{
	struct sstf_data *nd = q->elevator->elevator_data;

	if (rq->queuelist.next == &nd->queue)
		return NULL;
	return list_entry(rq->queuelist.next, struct request, queuelist);
}

static void *sstf_init_queue(struct request_queue *q)
{
	struct sstf_data *nd;

	nd = kmalloc_node(sizeof(*nd), GFP_KERNEL, q->node);
	if (!nd)
		return NULL;
	INIT_LIST_HEAD(&nd->queue);
	
	nd->direction = SSTF_UP;
	nd->current_pos = 0;

	return nd;
}

static void sstf_exit_queue(struct elevator_queue *e)
{
	struct sstf_data *nd = e->elevator_data;

	BUG_ON(!list_empty(&nd->queue));
	kfree(nd);
}

static struct elevator_type elevator_sstf = {
	.ops = {
		.elevator_merge_req_fn		= sstf_merged_requests,
		.elevator_dispatch_fn		= sstf_dispatch,
		.elevator_add_req_fn		= sstf_add_request,
		.elevator_former_req_fn		= sstf_former_request,
		.elevator_latter_req_fn		= sstf_latter_request,
		.elevator_init_fn		= sstf_init_queue,
		.elevator_exit_fn		= sstf_exit_queue,
	},
	.elevator_name = "sstf",
	.elevator_owner = THIS_MODULE,
};

static int __init sstf_init(void)
{
	elv_register(&elevator_sstf);

	return 0;
}

static void __exit sstf_exit(void)
{
	elv_unregister(&elevator_sstf);
}

module_init(sstf_init);
module_exit(sstf_exit);


MODULE_AUTHOR("GROUP 6");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SSTF Elevator IO scheduler");
