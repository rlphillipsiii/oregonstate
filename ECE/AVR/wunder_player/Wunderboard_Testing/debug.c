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
#include "debug.h"

unsigned char initializeUART(void){
	/* Set baud rate */
	UBRR1H = 0;
	UBRR1L = 12;
	
	/* Set the U2X1 bit */
	UCSR1A = (1 << U2X1);
	
	/* Enable transmitter */
	UCSR1B = (1 << TXEN1);
	
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = (1 << UCSZ11) | (1 << UCSZ10);
	
	return 0;
}

unsigned char sendByteUART(uint8_t data){
	while (!(UCSR1A & (1 << UDRE1)));

	UDR1 = data;

	return 0;
}

unsigned char sendStringUART(char * str){
	while(* str){
		sendByteUART(* str);
		str++;
	}
	
	return 0;
}

void sendDiskError(FRESULT err){
	if(err == FR_DISK_ERR)
		sendStringUART("hard error occurred in the low level disk I/O layer\r\n");
	else if(err == FR_INT_ERR)
		sendStringUART("Assertion failed\r\n");
	else if(err == FR_NOT_READY)
		sendStringUART("The physical drive cannot work\r\n");
	else if(err == FR_NO_FILE)
		sendStringUART("Could not find the file\r\n");
	else if(err == FR_NO_PATH)
		sendStringUART("Could not find the path\r\n");
	else if(err == FR_INVALID_NAME)
		sendStringUART("The path name format is invalid\r\n");
	else if(err == FR_DENIED)
		sendStringUART("Access denied due to prohibited access or directory full\r\n");
	else if(err == FR_EXIST)
		sendStringUART("Access denied due to prohibited access\r\n");
	else if(err == FR_INVALID_OBJECT)
		sendStringUART("The file/directory object is invalid\r\n");
	else if(err == FR_WRITE_PROTECTED)
		sendStringUART("The physical drive is write protected\r\n");
	else if(err == FR_INVALID_DRIVE)
		sendStringUART("The logical drive number is invalid\r\n");
	else if(err == FR_NOT_ENABLED)
		sendStringUART("The volume has no work area\r\n");
	else if(err == FR_NO_FILESYSTEM)
		sendStringUART("There is no valid FAT volume on the physical drive\r\n");
	else if(err == FR_MKFS_ABORTED)
		sendStringUART("The f_mkfs() aborted due to any parameter error\r\n");
	else if(err == FR_TIMEOUT)
		sendStringUART("Could not get a grant to access the volume within defined period\r\n");
	else if(err == FR_LOCKED)
		sendStringUART("The operation is rejected according to the file sharing policy\r\n");
	else if(err == FR_NOT_ENOUGH_CORE)
		sendStringUART("LFN working buffer could not be allocated\r\n");
	else if(err == FR_TOO_MANY_OPEN_FILES)
		sendStringUART("Number of open files > _FS_SHARE\r\n");
	else
		sendStringUART("Unknown SD Error\r\n");
}

void sendMountError(uint8_t err){
	if(err == ERR_FMOUNT)
		sendStringUART("Error mounting SD card\r\n");
	else if(err == ERR_NODISK)
		sendStringUART("No SD card detected\r\n");
	else if(err == ERR_NOINIT)
		sendStringUART("Unable to initialize FAT filesystem\r\n");
	else if(err == ERR_PROTECTED)
		sendStringUART("SD card is write protected\r\n");
	else
		sendStringUART("Unknown SD card error\r\n");
}