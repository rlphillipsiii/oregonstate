#define F_CPU 16000000L

#include <avr/io.h>
#include <util/delay.h>
#include "led_driver.h"

#define ZERO  ~0x3F // 0b00111111
#define ONE   ~0x06 // 0b00000110
#define TWO   ~0x5B // 0b01011011
#define THREE ~0x4F // 0b01001111
#define FOUR  ~0x66 // 0b01100110
#define FIVE  ~0x6D // 0b01101101
#define SIX   ~0x7C // 0b01111100
#define SEVEN ~0x07 // 0b00000111
#define EIGHT ~0x7F // 0b01111111
#define NINE  ~0x67 // 0b01100111
#define OFF   ~0x00 // 0b00000000

#define COLON_ONLY ~0x03 // 0b00000011
#define COLON_ALL  ~0x07 // 0b00000111
#define DOT_ONLY   ~0x04 // 0b00000100

#define BLANK 10

#define ZERO_FILLED 1
#define ZERO_BLANK  0

static uint8_t NUMBERS[] = { ZERO, ONE, TWO, THREE, FOUR, FIVE,
							 SIX, SEVEN, EIGHT, NINE, OFF };

void enable_leds()
{
	// disable the tristate buffer without enabling a segment
	PORTB &= ~(1 << PB4);
	PORTB |= (1 << PB6) | (1 << PB5);

	DDRA = 0xFF; // make PORTA all outputs
	PORTA = 0xFF;
}

void disable_leds()
{
	PORTB &= ~(1 << PB4);
    PORTB |= (1 << PB6) | (1 << PB5);
}

void init_display_struct(struct display *output)
{
	output->ones = 0;
	output->tens = 0;
	output->hundreds = 0;
	output->thousands = 0;

	output->colon = COLON_ONLY;
	output->mode = ZERO_BLANK;
	output->cycle = 0;
}

void update_colon(struct display *output, uint8_t off, uint8_t all)
{
	if (!off) {
		if (all)
			output->colon = COLON_ALL;
		else
			output->colon = COLON_ONLY;
	} else {
		output->colon = OFF;
	}
}

void set_colon(struct display *output)
{
	PORTA = output->colon;
}

void set_zero_filled(struct display *output)
{
	output->mode = ZERO_FILLED;
}

void decode_number(struct display *output, uint16_t number)
{
	output->ones = (number%10);
	output->tens = (number%100)/10;
	output->hundreds = (number%1000)/100;
	output->thousands = number/1000;

	if (output->mode == ZERO_FILLED)
		return;

	if (output->thousands != 0)
		return;

	output->thousands = BLANK; // turn of digit 4 if thousands is 0
	if (output->hundreds != 0)
		return;

	output->hundreds = BLANK; // turn of digit 3 if hundreds is 0
	if (output->tens != 0)
		return;

	output->tens = BLANK; // turn of digit 2 if tens is 0
}

void enable_segment(uint8_t segment)
{
	switch (segment) {
	case 0: // digit 1 enabled: PORTB = 0b?000????
		PORTB &= ~(1 << PB4) & ~(1 << PB5) & ~(1 << PB6);
		break;

	case 1: // digit 2 enabled: PORTB = 0b?001????
		PORTB &= ~(1 << PB5) & ~(1 << PB6);
		PORTB |= (1 << PB4);
		break;

	case 2: // colons enabled: PORTB = 0b?010????
		PORTB &= ~(1 << PB4) & ~(1 << PB6);
		PORTB |= (1 << PB5);
		break;

	case 3: // digit 3 enabled: PORTB = 0b?011????
		PORTB &= ~(1 << PB6);
		PORTB |= (1 << PB4) | (1 << PB5);
		break;

	case 4: // digit 4 enabled: PORTB = 0b?100????
		PORTB &= ~(1 << PB4) & ~(1 << PB5);		
		PORTB |= (1 << PB6);
		break;
	}
}

void set_digit(uint8_t digit)
{
	PORTA = NUMBERS[digit];
}
 
void update_segment(struct display *output, uint16_t number)
{
	decode_number(output, number); // get the 4 digits 

	set_digit(BLANK); // blank before switching to prevent ghosting
	enable_segment(output->cycle); // enable the correct digit

	switch(output->cycle) { // display the digit based on the segment enabled
	case 0: set_digit(output->ones);      break; // ones digit
	case 1: set_digit(output->tens);      break; // tens digit
	case 2: set_colon(output);             break; // colons
	case 3: set_digit(output->hundreds);  break; // hundreds digit
	case 4: set_digit(output->thousands); break; // thousands digit
	}

	output->cycle++; // go to the next segment
	if (output->cycle > 4)
		output->cycle = 0; // go back to the beginning if at the last segment	
}
