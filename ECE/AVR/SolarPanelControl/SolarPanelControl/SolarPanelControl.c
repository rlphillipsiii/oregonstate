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
 * Includes functions for sending and receiving data to and from the computer
 * and moving the motor accordingly
 * 
 * @section HARDWARE
 * Target Device: ATTINY261
 */

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>

#include "motor.h"

/** 
 * Flag that is set when the connection command is received from the computer
 */
int connection = 0;

/** 
 * Holds the index of the next SPI byte in the packet
 */
unsigned char count;

/** 
 * Holds the SPI packet
 */
unsigned char data[6];

/** 
 * Holds the next value to be sent out via SPI
 */
unsigned char data_out = 0;

/**
 * Holds the latest ADC value from the solar panel
 */
unsigned char solar_reading;

/**
 * Holds the current destination request for the motor from the computer
 */
unsigned char position = 255;

/**
 * Holds the last destination request for the motor from the computer
 */
unsigned char last = 255;

/** 
 * Interrupt to indicate the end of serial transmission 
 *
 * The data buffer is updated with the latest SPI bytes.  If a new packet
 * is arriving, a new reading of the solar ADC is taken and sent back to the
 * computer.  The position variable is also updated, which signals the
 * motors to move when the interrupt is finished.
 *
 * @see ADC_REF_25
 * @see SOLAR_ADC
 */
ISR(USI_OVF_vect){
	data[count] = USIDR; // Data Register - currently holds input data
	USIDR = data_out; // Set the output data to the data register
	
	USISR = 1 << USIOIF; // Clear the interrupt flag
	
	if (count == 1){
		ADCSetReference(ADC_REF_25);
		solar_reading = ReadADC(SOLAR_ADC);
	}
	
	data_out = solar_reading;
	
	position = data[0];
	count++;
		
	if (count == 5)
		data_out = 0xA9; // First byte out of next packet	
	
	if (count == 6)
		count = 0;
		
	connection = 1;
	return;
}


/** 
 * Enables the Tiny to be an SPI slave
 */
void SPI_SlaveInit(){
	USICR = (1 << USIWM0) | (1 << USICS1) | (1 << USIOIE);
	
	USISR = 1 << USIOIF; // Clear the interrupt flag, reset the counter
}

/**
 * Finds the position of the solar panel that gives the highest ADC reading
 *
 * The Tiny loops through each position (LOWER_BOUND to UPPER_BOUND) in increments
 * of 20 looking for the position with the highest ADC reading.  When all the
 * measurements have been taken, the motor moves the solar panel to the optimal
 * position.  While the Tiny is searching for the optimal position, interrupts are
 * disabled until the process is finished.
 *
 * @see ADC_REF_25
 * @see SOLAR_ADC
 * @see LOWER_BOUND
 * @see UPPER_BOUND
 * @see SERVO_POSITION_TIMEOUT
 */
void find_brightest_light(){
	cli();
	
	uint8_t max = 0, tmp = 0;
	uint16_t pos, i;
	
	for(i = LOWER_BOUND; i < UPPER_BOUND; i += 20){
		set_servo_position(i, SERVO_POSITION_TIMEOUT);
		
		ADCSetReference(ADC_REF_25);
		if((tmp = ReadADC(SOLAR_ADC)) > max){
			max = tmp;
			pos = i;
		}			
	}		
	
	set_servo_position(pos, SERVO_POSITION_TIMEOUT);
	
	sei();
}

/** 
 * Initializes the Tiny to wait for commands from the computer and move the motor accordingly
 *
 * The position variable is continually polled to determine whether or not the position of the
 * motor needs to be updated.  The ADC, PWM, and SPI are also enabled at the beginning.
 *
 * @see ADC_PRESCALER_4
 * @see SERVO_POSITION_TIMEOUT
 */
int main (void) {
	DDRA = 0b00000111;
	DDRB = (1 << PB1) | (1 << PB3) | (1 << PB5);
	
	SetupADC(ADC_PRESCALER_4, FALSE);
 	ADCEnable();
		
	enable_PWM();
	SPI_SlaveInit();
	
	sei();
	
 	USIDR = 0xA9;
  	TCNT1 = 0x00;
	
	while(1){
		if(connection){
			if(position != last){
				if(position == 255){
					find_brightest_light();
				}else{
					cli();
					
					set_servo_position(position, SERVO_POSITION_TIMEOUT);	
					sei();		
				}
								
				last = position;
			}	
		}						
			
		_delay_ms(1);
	}		
	
}
