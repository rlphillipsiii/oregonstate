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
 * .wav file decoding and playback functions
 *
 * @section HARDWARE
 * Target Device: Wunderboard
 */

#include "player.h"

uint8_t initializeFAT(FATFS* fs){
	DSTATUS driveStatus;
	
	/* Mount and verify disk type */
	if (f_mount(0, fs) != FR_OK)
		return ERR_FMOUNT;
	
	driveStatus = disk_initialize(0);
	
	/* Verify that disk exists */
	if (driveStatus & STA_NODISK)
		return ERR_NODISK;
	
	/* Verify that disk is initialized */
	if (driveStatus & STA_NOINIT)
		return ERR_NOINIT;
	
	/* Verify that disk is not write protected */
	if (driveStatus & STA_PROTECT)
		return ERR_PROTECTED;
	
	return ERR_NONE;
}

void seek_to_data(const char * path){
	char buf[1];
	FRESULT result;
	
	/* open the .wav file for reading */
	if((result = f_open(&wav, path, FA_READ | FA_OPEN_EXISTING)) != FR_OK){
		#ifdef DEBUG
		sendDiskError(result);
		#endif
		return;
	}
	
	/* define local variables */
	char * flag = "data";
	int i, found = 0;

#ifdef DEBUG
	/* send the computer an update via UART */
	sendStringUART("Reading file...\r\n");
#endif

	/* find the data section */
	do{
		/* read one byte at a time */
		f_read(&wav, (void *) buf, 1, &bytes_read);
		
		/* if a 'd' is found, check to see if the word "data" has been found */
		i = 0;
		while(buf[0] == flag[i]){
			i ++;
			if(i == 4){
				found = 1;
				break;
			}
			
			f_read(&wav, (void *) buf, 1, &bytes_read);
		}
	}while(bytes_read > 0 && !found);
	
#ifdef DEBUG
	sendStringUART("Data section found!!\r\n");
#endif

	/* eat up the next four bytes that correspond to the data section size */
	for(i = 0; i < 4; i++)
		f_read(&wav, (void *) buf, 1, &bytes_read);
	
	/* read in the first 2048 bytes of the song */
	FRESULT error = f_read(&wav, (void *) sample, 2048, &bytes_read);
	
#ifdef DEBUG
	if(error != 0)
		sendDiskError(error);

	sendStringUART("Initialization of wav file finished!\r\n");
#endif
}

void play_wav(const char * filename, int alarm){
	/* advance the .wav file pointer to the music data section */
	seek_to_data(filename);
		
	song_index = 0;
	
	/* Set OCR1A low on compare match and high when the top is reached */
	TCCR1A |= (1 << WGM10) | (1 << COM1A1);
	
	/* Clk/1 and 8 bit Fast PWM mode 5 */
	TCCR1B |= (1 << WGM12) | (1 << CS10);

	/* Set OCR0A to normal operation mode */
	TCCR0A = 0b00000000;

	/* Set prescaler to Clk/64 */
	TCCR0B |= (1 << CS01) | (1 << CS00);
		
	/* enable OCR0A timer counter compare interrupts */
	TIMSK0 |= (1 << OCIE0A);
	
	/* set the interrupt timing to match the sample rate */
	OCR0A = 15;
	TCNT0 = 0;	
	
    while(1) {
		/* 
		 * check if the index needs to be reset, which indicates that the samples
		 * have all been played and 2048 new samples need to be read in
		 */
		if(song_index >= 2048 && bytes_read > 0){
			song_index = 0;
			
			/* read 2048 samples into the buffer */
			while(f_read(&wav, (void *) sample, 2048, &bytes_read) != 0);
			
			/* sound is finished playing so disable the sampling interrupt and return */
			if(bytes_read == 0){
				TIMSK0 &= ~(1 << OCIE0A);
				
				return;
			}				
		}else if(stopPlay && !alarm){
			TIMSK0 &= ~(1 << OCIE0A);
			
			play_wav("/alarm.wav", 1);	
			stopPlay = 0;
			
			return;
		}
		
		_delay_us(1);
	}	
}
