// testled1.c
// Robert Phillips III
// 10.3.05
// Tests wiring of LED board to mega128.
// Select a digit with PORTB upper nibble then with
// port D push buttons illuminate a single segment.

// Port mapping:
// Port A:  bit0 black  segment A
//          bit1 white  segment B
//          bit2 grey   segment C
//          bit3 purple segment D
//          bit4 blue   segment E
//          bit5 green  segment F
//          bit6 yellow segment G
//          bit7 orange decimal point
//               brown  Vdd
//               red    GND

// Port B:  bit4 white  seg0
//          bit5 purple seg1
//          bit6 green  seg2
//          bit7 black  pwm 
//               brown  Vdd
//               orange GND

#define F_CPU 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <led_driver.h>
#include <button_driver.h>

#define ISSET(byte, bit) (byte & (1 << bit))

uint8_t values[] = {1, 2, 4, 8, 16, 32, 64, 128};
                                  
int main()
{
	DDRB  = 0xF0;   //set port bits 4-7 B as outputs
	DDRD  = 0x00;   //set port D all inputs 
	PORTD = 0xFF;   //set port D all pullups 

	struct display output;
	init_display_struct(&output); // init the struct the holds the display state

	enable_leds(); // enable the display

	uint16_t number = 0; // init the number on the display to show 0

	while (1) {	
		_delay_us(300);

		enable_buttons(); // enable the buttons and disable the display

		char pina = poll_buttons(PINA); // get the button state
		
		uint8_t i;
		for (i = 0; i < 8; i++) {
			if (ISSET(pina, i)) {
				number += values[i]; // add the button's value if it's pressed

				if (number > 1023) // rollover
					number = number-1023;
			}
		}

		update_segment(&output, number); // continuously update the display
		enable_leds(); // re-enable the display and disable the buttons
	}  //while
}  //main
