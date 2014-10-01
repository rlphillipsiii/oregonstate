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
#include <graph_driver.h>
#include <encoder_driver.h>
#include <macros.h>

volatile uint16_t number;
volatile char pina;
volatile char mode;

uint8_t values[] = {1, 2, 4, 8, 16, 32, 64, 128};
                                  
/******************************************************
 * Initializes an overflow interrupt on timer 0 that
 * controls the frequency at which the buttons are
 * checked for its pressed state.
 ******************************************************/
void init_timer()
{
	TIMSK |= (1 << TOIE0);             //enable interrupts
	TCCR0 |= (1 << CS02) | (1 << CS00);  //normal mode, prescale by 128
}

uint16_t update_number(struct encoder_state *state, uint16_t number)
{
	if (mode == 0x03)
		return number; // both modes are active so do nothing

	if (state->left_dir == STOPPED && state->right_dir == STOPPED)
		return number; // the encoders are stopped so do nothing

	/* set the increment based on the current mode */
	uint8_t increment = 0;
	switch(mode) {
	case 0x00: increment = 1; break;
	case 0x01: increment = 2; break;
	case 0x02: increment = 4; break;
	}

	if (state->left_dir == FORWARD || state->right_dir == FORWARD) {
		number += increment; // add the increment if either encoder is going forward
		if (number > 1023)
			number -= 1023; // rollover positive
	}else if (state->left_dir == REVERSE || state->right_dir == REVERSE) {
		if (number < increment)
			number += 1024; // rollover negative

		number -= increment; // sub the increment if either encoder is going reverse
	}

	return number;
}

char set_graph(char buttons, char mode)
{
	if (ISSET(buttons, 0))
		mode ^= 0x01; // toggle the mode

	if (ISSET(buttons, 1))
		mode ^= 0x02; // toggle the mode

	update_graph(mode); // update the graph with the mode
	return mode;
}

/******************************************************
 * The interrupt vector that checks the state of the
 * buttons and alters the displayed number accordingly.
 ******************************************************/
ISR(TIMER0_OVF_vect)
{
	enable_buttons(); // enable the buttons and disable the display
	pina = poll_buttons(PINA); // get the button state
	enable_leds(); // re-enable the leds

	mode = set_graph(pina, mode); // update the mode
}

int main()
{
	DDRB  = 0xF0;   //set port bits 4-7 B as outputs
	DDRD  = 0x00;   //set port D all inputs 
	PORTD = 0xFF;   //set port D all pullups 

	struct display output;
	init_display_struct(&output); // init the struct the holds the display state

	pina = 0xFF;
	init_timer(); // init the timer to check for the button states

	enable_leds(); // enable the display

	enable_graph(); // enable the bar graph
	update_graph(OFF); // make sure the bar graph is off

	enable_encoders(); // enable the encoders

	struct encoder_state encoders;
	init_encoder_struct(&encoders);// init the encoder data struct

	uint16_t number = 0; // init the number on the display to show 0
	mode = 0;

	sei(); 
	while (1) {	
		_delay_us(50); // loop delay for the segments
		
		get_direction(&encoders, poll_encoders()); // get the encoder direction
		number = update_number(&encoders, number); // update the led display number

		update_segment(&output, number); // continuously update the display
	}  //while
}  //main
