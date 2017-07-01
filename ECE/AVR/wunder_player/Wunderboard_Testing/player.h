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


#ifndef PLAYER_H_
#define PLAYER_H_

#include "ff.h"
#include "debug.h"

/** 
 * .wav file handle
 */
FIL wav;

/** 
 * The index in the buffer of the next sample to be played
 */
int song_index;

/** 
 * Flag that indicates when playback of a .wav file should be interrupted
 */
volatile int stopPlay;

/** 
 * Flag that indicates when playback of a .wav file should begin
 */
volatile int playback;

/** 
 * FAT filesystem handle
 */
FATFS fat;

/** 
 * Buffer that holds the .wav file samples
 */
unsigned char sample[2048];

/** 
 * The number of bytes last read from the file
 */
UINT bytes_read;


/** 
 * Sets the CPU prescale options
 */
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))

/** 
 * Initializes the FAT filesystem for IO.
 *
 * @param fs The FATFS pointer to store the FAT handle
 *
 * @see ERR_FMOUNT
 * @see ERR_NODISK
 * @see ERR_NOINIT
 * @see ERR_PROTECTED
 * @see ERR_NONE
 */
uint8_t initializeFAT(FATFS* fs);


/** 
 * Opens the .wav file and advances it past the header to the
 * data section that contains the audio samples
 *
 * @param path The file path of the .wav file to be played
 *
 * @see FA_READ
 * @see FA_OPEN_EXISTING
 */
void seek_to_data(const char * path);


/** 
 * Decodes the specified .wav file and reads the samples into a
 * buffer in groups of 2048
 *
 * If the alarm flag has not been set, the alarm flag is polled
 * and if it is set at any time during the playback, the current
 * .wav file playback will end immediately and playback of the alarm
 * will begin.  The playback of the alarm can not be interrupted.
 *
 * @param filename The file path of the .wav file to be played
 * @param alarm The flag that indicates whether or not an alarm is
 * being played
 */
void play_wav(const char * filename, int alarm);


#endif /* PLAYER_H_ */