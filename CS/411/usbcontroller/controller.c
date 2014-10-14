#include <usb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

/* timeout in ms for the USB control transfer */
#define TIMEOUT_MS 3000

/* the amount of time to move before sending the stop command */
#define COMMAND_MS 50

/* the vendor id of the USB launcher */
#define VENDORID 0x2123

/* the bm request type as determined using snoopypro */
#define BM_REQUEST_TYPE 0x21

/* the b request value as determined using snoopypro */
#define B_REQUEST 0x09

/* the value for the control message as determined using snoopypro */
#define VALUE 0x200

/* the index for the control message as determined using snoopypro */
#define INDEX 0x00

/* readability defines for the launcher's movements */
#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3
#define STOP  4


/*
 * Sleeps for at least the given number of millis
 */
void sleep_ms(int millis)
{
  /* convert the number of ms to ns */
  long nanos = 1000000L*millis;

  /* init the timespec structs to the delay */
  struct timespec req, rem;
  req.tv_sec = 0;
  req.tv_nsec = nanos;

  rem.tv_sec = 0;
  rem.tv_nsec = 0;

  /* sleep for the given time */
  nanosleep(&req, &rem);
}


/*
 * Returns a handle representing the device with the given
 * vendorID.  If the device is not found, the program exits 
 * with a code indicating abnormal exit.
 */
usb_dev_handle *get_usb_handle(int vendorID)
{
  struct usb_bus *bus;

  /* initialize the USB stuff */
  usb_init();

  /* find all the USB busses and devices */
  usb_find_busses();
  usb_find_devices();

  /* loop through all the busses */
  for (bus = usb_get_busses(); bus; bus = bus->next) {
    struct usb_device *dev;

    /* loop through all the devices on the current bus */
    for (dev = bus->devices; dev; dev = dev->next) {
      /* return an open USB handle if the vendor ID's match */
      if (dev->descriptor.idVendor == vendorID)
	return usb_open(dev);
    }
  }
  
  /* error out if the device wasn't found */
  printf("Couldn't find launcher!  Is it plugged in?\n");
  exit(EXIT_FAILURE);
}


/*
 * Sends a control message to the device represented by the
 * usb_dev_handle.  If there is an error sending the USB 
 * message, then the program will execute with an exit code
 * corresponding to an abnormal exit.
 */
void usb_msg(usb_dev_handle *handle, char *msg)
{
  /* send a USB control message to the launcher */
  if (usb_control_msg(handle,
		      BM_REQUEST_TYPE,
		      B_REQUEST,
		      VALUE, 
		      INDEX, 
		      msg,
		      sizeof(msg), 
		      TIMEOUT_MS) < 0) {
    /* error out if the control message failed to send */
    perror("Couldn't send control message");
    exit(EXIT_FAILURE);
  }
}


/*
 * Init is a special case of the move function.  It operates
 * in the same exact way that the move function does.  It is
 * separated out for clarity.
 */
void launcher_init(usb_dev_handle *handle)
{
  /* initialize the transfer buffer */
  char msg[8];
  memset(msg, 0, sizeof(msg));

  /* load the initialization command */
  msg[0] = 0x03;
  msg[1] = 0x01;

  /* send the command to the launcher */
  usb_msg(handle, msg);
}


/*
 * Fire is a special case of the move function.  It operates
 * in the same exact way except that the process sleeps for 3
 * seconds so the fire command is not prematurely interrupted
 * by a STOP command.  It is separate from the move function
 * for clarity.
 */
void fire(usb_dev_handle *handle)
{
  /* initialize transfer buffer */
  char msg[8];
  memset(msg, 0, sizeof(msg));

  /* load the buffer with the fire command */
  msg[0] = 0x02;
  msg[1] = 0x10;

  /* send the command to the launcher */
  usb_msg(handle, msg);

  /* wait for the launcher to finish firing */
  sleep(3);
}


/*
 * Helper function that builds the motion command and sends
 * it via USB to the launcher.
 */
void _move(usb_dev_handle *handle, int dir)
{
  /* Commands are an 8 byte message */
  char msg[8];

  /* zero out the buffer for sending */
  memset(msg, 0, sizeof(msg));
  
  /* all commands start with 0x02 */
  msg[0] = 0x02;

  /* alter the second byte to match the desired direction */
  switch (dir) {
  case LEFT:
    msg[1] = 0x04;
    break;
    
  case RIGHT:
    msg[1] = 0x08;
    break;
    
  case UP:
    msg[1] = 0x02;
    break;
    
  case DOWN:
    msg[1] = 0x01;
    break;
  }

  /* send the command to the launcher */
  usb_msg(handle, msg);      
}

/*
 * Moves the launcher in the given direction for an amount
 * of time given by the COMMAND_MS define.  A stop command
 * is then issued if specified that it should.  If a stop 
 * command is not issued, the given command will execute 
 * until the command is no longer valid (motion past a boundary).
 */
void move(usb_dev_handle *handle, int dir, int stop)
{
  /* move it in the specified direction */
  _move(handle, dir);

  if (stop) {
    /* wait a certain number of millis */
    sleep_ms(COMMAND_MS);

    /* stop moving */
    _move(handle, STOP);
  }
}

/*
 * Entry point for the program.  Puts everything together.
 */
int main(int argc, char **argv)
{
  /* flag indicating whether or not an automatic stop command is sent */
  int auto_stop = 1;
  
  if (argc < 3)
    exit(EXIT_FAILURE);

  /* get a USB handle for the launcher */
  usb_dev_handle *handle = get_usb_handle(VENDORID);

  /* detach the default kernel driver if not already */
  if (usb_detach_kernel_driver_np(handle, 0)) {
    printf("Kernel driver already detached\n");
  }

  /* claim interface 0 of the launcher */
  if (usb_claim_interface(handle, 0) < 0) {
    printf("Couldn't claim USB device\n");
    exit(EXIT_FAILURE);
  }

  /* check to see if an automatic stop should be issued */
  if (argv[2][0] == 'n')
    auto_stop = 0;
  
  /* execute the command indicated by the command line param */
  switch (argv[1][0]) {
  case 'f':
    fire(handle);
    return 0;

  case 'u':
    move(handle, UP, auto_stop);
    return 0;

  case 'd':
    move(handle, DOWN, auto_stop);
    return 0;

  case 'l':
    move(handle, LEFT, auto_stop);
    return 0;

  case 'r':
    move(handle, RIGHT, auto_stop);
    return 0;
  }
}
