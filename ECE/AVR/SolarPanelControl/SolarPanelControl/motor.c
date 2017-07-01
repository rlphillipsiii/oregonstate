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
 * Includes functions for setting up PWM on PB5 and controlling the motor
 * that is to have a solar panel mounted on it
 * 
 * @section HARDWARE
 * Target Device: ATTINY261
 */


#include "motor.h"

void set_motor_forward(){
	PORTA |= (1 << PA1);
	PORTA &= ~(1 << PA2);
}

void set_motor_reverse(){
	PORTA &= ~(1 << PA1);
	PORTA |= (1 << PA2);
}

void set_motor_stop(){
	PORTA &= ~(1 << PA1);
	PORTA &= ~(1 << PA2);
}

void set_direction(int direction){
	if(direction == FORWARD){
		PORTA |= (1 << PA1);
		PORTA &= ~(1 << PA2);
	}else{
		PORTA &= ~(1 << PA1);
		PORTA |= (1 << PA2);
	}
}

void move_motor(int dir){
	if (dir == CCW){
		PWMD = SERVO_SPEED;
		set_motor_reverse();
	}else if (dir == CW){
		PWMD = SERVO_SPEED;
		set_motor_forward();
	}else{
		PWMD = 0;
		set_motor_stop();
	}
}

int get_error(uint8_t destination){
	uint8_t position;
	
	ADCSetReference(ADC_REF_VCC);
	position = ReadADC(POT_ADC);
	
	if (destination > UPPER_BOUND)
		destination = UPPER_BOUND;	// Set it to the upper bound
	else if (destination < LOWER_BOUND)
		destination = LOWER_BOUND;	// Set it to the lower bound
	
	return position - destination;
}

void update_servo_position(uint8_t destination){
	int error = get_error(destination);
	
	if (ABS(error) < DEADBAND)	// If the error is less than the deadband, just turn off the motor.
		move_motor(STOP);
	else if (error > 0)	// If the error is negative, turn the servo clockwise.
		move_motor(CCW);
	else	// Otherwise turn the servo counter-clockwise.
		move_motor(CW);
}

void set_servo_position(uint8_t destination, uint16_t seekTime){
	if(destination == 0)
		return;
	
	uint16_t i;
	
	for (i = 0; i < seekTime; i++){
		update_servo_position(destination);
		
		_delay_ms(1);
	}
}

void enable_PWM(){
	TOP = 0xFF;
	
	TCCR1B = (1 << CS10);
	TCCR1C = (1 << PWM1D) | (1 << COM1D1);
	TCCR1D &= ~(1 << WGM10) & ~(1 << WGM11);
}