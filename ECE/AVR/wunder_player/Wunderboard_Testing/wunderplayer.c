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
 * Requires 8-bit unsigned WAV files with a sample rate of
 * 8kHz due to the limitations of using an AT90USB647 and
 * PWM as a WAV decoder.
 * 
 * @section HARDWARE
 * Target Device: Wunderboard
 */

#include <avr/interrupt.h>

#include "player.h"

#define DEBUG 0
volatile unsigned char init = 0;

/** 
 * Initializes the ports to their correct input or output setting
 */
void initialize (void) {
	/* Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/* Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11100111;

	/* Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;
	
	/* Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00000000;
	PORTD |= (1 << PB0) | (1 << PB1);
	
	/* Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/* Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}


/** 
 * Enables external interrupts INT0 and INT1 (PD0 and PD1)
 */
void enable_ext_interrupts(){
	playback = stopPlay = 0;
	
	/* 
	 * disable INT0 and INT1 so an interrupt isn't triggered when we change the
	 * interrupt trigger to the rising edge
	 */ 
	EIMSK &= ~(1 << INT0) | ~(1 << INT1);
	
	/* trigger the interrupt on the rising edge of the pin change for INT0 and INT1 */
	EICRA |= (1 << ISC00) | (1 << ISC01) | (1 << ISC11) | (1 << ISC10);
	
	/* enable external interrupt 1 and interrupt 0 */
	EIMSK |= (1 << INT0);// | (1 << INT1);
	
	/* globally enable interrupts */
	sei();
}

/** 
 * External interrupt for PD0 that sets the playback flag, which begins
 * playback of the .wav file if it is not already playing
 */
SIGNAL(INT0_vect){
	if (init == 0) {
		init = 1;
		playback = 0;
	} else {
		if(!playback){
			playback = 1;
			
#ifdef DEBUG
	sendStringUART("Playback interrupt...\r\n");
#endif
		}
	}	
}

/** 
 * External interrupt for PD1 that sets the stopPlayback flag, which interrupts
 * the playback of the main .wav file and begins playback of the alarm .wav file
 * if it is not already playing.
 */
SIGNAL(INT1_vect){
#ifdef DEBUG
	sendStringUART("Alarm interrupt...\r\n");
#endif

	if(playback)
		stopPlay = 1;
	else{
		playback = 1;
		play_wav("/alarm.wav", 1);
		
		stopPlay = playback = 0;
	}		
}


/** 
 * Timer counter compare interrupt that has a frequency close to 8000 Hz that changes
 * the duty cycle of the PWM to match the corresponding sample and generate audio
 */
SIGNAL(TIMER0_COMPA_vect){
	/* reset the interrupt timer */
	TCNT0 = 0;	
	
	/* change the PWM to match the voltage of the next sample */
	OCR1A = sample[song_index];
	
	/* increment the sample index */
	song_index ++;
	return;
}


/** 
 * Polls the playback flag and begins audio playback when the playback flag
 * is set
 */
int main(void) {	
	CPU_PRESCALE(0);
	
	initialize();
	
#ifdef DEBUG
	initializeUART();
	sendStringUART("Mounting SD card and opening file...\r\n");
#endif

	/* mount the SD card and initialize the file system */
	FRESULT err;
	if((err = initializeFAT(&fat)) != ERR_NONE){
#ifdef DEBUG
		sendMountError(err);
#endif
		return 1;
	}else{
#ifdef DEBUG
		sendStringUART("SD card mounted\r\n");
#endif
	}
		
	enable_ext_interrupts();
	
    while(1) {
		if(playback)
			play_wav("/message.wav", 0);
			
		playback = 0;
		
		_delay_ms(1);
	}
		
	return 1;
}