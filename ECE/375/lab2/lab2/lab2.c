/*
 * lab2.c
 *
 *  Created: 9/25/2012 2:27:20 PM
 *  Author: Robert Phillips III
 */ 


#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEFT 3
#define RIGHT 2
#define FORWARD 1
#define REVERSE 0

#define RIGHT_ENABLE PB4
#define LEFT_ENABLE PB7
#define RIGHT_DIR PB5
#define LEFT_DIR PB6

unsigned char left, right;

void setup() {
	left = 0;
	right = 0;
	
	EICRA = (1 << ISC01) | (1 << ISC11);
	EIMSK = (1 << INT1) | (1 << INT0);
	
	DDRA = 0b11111100;
	DDRB = 0b11110000;
	
	PORTB = 0;
	sei();
}

SIGNAL(SIG_INTERRUPT0){
	right = 1;
	
	return;
}

SIGNAL(SIG_INTERRUPT1){
	left = 1;
	
	return;
}

void set_right_motor(int dir){
	if(dir == FORWARD){
		PORTB |= (1 << RIGHT_DIR);
		PORTB &= ~(1 << RIGHT_ENABLE);
	}else{
		PORTB &= ~(1 << RIGHT_ENABLE) & ~(1 << RIGHT_DIR);
	}	
}

void set_left_motor(int dir){
	if(dir == FORWARD){
		PORTB |= (1 << LEFT_DIR);
		PORTB &= ~(1 << LEFT_ENABLE);
	}else{
		PORTB &= ~(1 << LEFT_ENABLE) & ~(1 << LEFT_DIR);
	}
}

void set_direction(int dir){
	switch(dir){
		
		case FORWARD:	
			set_right_motor(FORWARD);
			set_left_motor(FORWARD);
			break;
			
		case REVERSE:
			set_right_motor(REVERSE);
			set_left_motor(REVERSE);
			break;
			
		case LEFT:
			set_right_motor(FORWARD);
			set_left_motor(REVERSE);
			break;
			
		case RIGHT:
			set_right_motor(REVERSE);
			set_left_motor(FORWARD);
			break;
			
	}
}

int main(void)
{
	setup();
	
    while(1) {
        if(right == 1){
			_delay_ms(1000);
			
			set_direction(REVERSE);
			_delay_ms(1000);
			
			set_direction(LEFT);
			_delay_ms(250);
			
			left = 0;
			right = 0;
		}else if(left == 1){
			_delay_ms(1000);
			
			set_direction(REVERSE);
			_delay_ms(1000);
			
			set_direction(RIGHT);
			_delay_ms(250);
			
			left = 0;
			right = 0;
		}else{
			set_direction(FORWARD);	
		}						
		
		_delay_ms(1);
    }
}