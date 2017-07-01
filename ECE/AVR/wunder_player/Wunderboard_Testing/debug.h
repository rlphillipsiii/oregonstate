/** 
 * @file
 * @author Robert L. Phillips III
 * @version 1.1
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * @section DESCRIPTION
 * 
 * Debugging helper functions
 * 
 * @section HARDWARE
 * Target Device: Wunderboard
 */


#ifndef DEBUG_H_
#define DEBUG_H_

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "diskio.h"
#include "ff.h"

/* enable UART debugging messages */
//#define DEBUG 0

/** 
 * No errors opening the filesystem 
 */
#define ERR_NONE 0x00

/** 
 * Failed to mount SDC/MMC error code 
 */
#define ERR_FMOUNT 0x01

/** 
 * No SDC/MMC present error code 
 */
#define ERR_NODISK 0x02

/** 
 * Unable to initialize FAT file system error code 
 */
#define ERR_NOINIT 0x03

/** 
 * MMC/SDC write protected error code 
 */
#define ERR_PROTECTED 0x04

#define ERR_FOPEN 0x05
#define ERR_TIMER 0x06
#define ERR_FWRITE 0x07
#define ERR_FULL 0x08
#define ERR_FCLOSE 0x09
 
/** 
 * Initializes UART so that debugging messages can be sent to the
 * computer via serial communication
 *
 * @return Always returns 0
 */
unsigned char initializeUART(void);


/** 
 * Sends a byte to the computer via UART
 *
 * @param data The byte to be sent to the computer 
 *
 * @return Always returns 0
 */
unsigned char sendByteUART(uint8_t data);


/** 
 * Sends a string to the computer via UART
 *
 * @param str The string to be sent to the computer
 *
 * @return Always returns 0
 */
unsigned char sendStringUART(char * str);


/**
 * Sends an error message to the computer depending on the supplied
 * error code that resulted from attempting to read from the SD/MMC
 *
 * @param err The error code
 *
 * @see FR_DISK_ERR
 * @see FR_INT_ERR
 * @see FR_NOT_READY
 * @see FR_NO_FILE
 * @see FR_NO_PATH
 * @see FR_INVALID_NAME
 * @see FR_DENIED
 * @see FR_EXIST
 * @see FR_INVALID_OBJECT
 * @see FR_WRITE_PROTECTED
 * @see FR_INVALID_DRIVE
 * @see FR_NOT_ENABLED
 * @see FR_NO_FILESYSTEM
 * @see FR_MKFS_ABORTED
 * @see FR_TIMEOUT
 * @see FR_LOCKED
 * @see FR_NOT_ENOUGH_CORE
 * @see FR_TOO_MANY_OPEN_FILES
 */
void sendDiskError(FRESULT err);


/** 
 * Sends an error message to the computer depending on the supplied error
 * code that resulted from attempting to mount the filesystem
 * 
 * @param err The error code
 *
 * @see ERR_NONE
 * @see ERR_FMOUNT
 * @see ERR_NODISK
 * @see ERR_NOINIT
 * @see ERR_PROTECTED
 */
void sendMountError(uint8_t err);


#endif /* DEBUG_H_ */