// lab1_code.c 
// R. Traylor
// 7.21.08

//This program increments a binary display of the number of button pushes on switch 
//S0 on the mega128 board.

#define F_CPU 16000000UL     //16Mhz clock
#include <avr/io.h>
#include <util/delay.h>

struct segment {
	uint8_t ones;
	uint8_t tens;
};

//*******************************************************************************
//                            debounce_switch                                  
// Adapted from Ganssel's "Guide to Debouncing"            
// Checks the state of pushbutton S0 It shifts in ones till the button is pushed. 
// Function returns a 1 only once per debounced button push so a debounce and toggle 
// function can be implemented at the same time.  Expects active low pushbutton on 
// Port D bit zero.  Debounce time is determined by external loop delay times 12. 
//*******************************************************************************
int8_t debounce_switch() 
{
	static uint16_t state = 0; //holds present state
	state = (state << 1) | (! bit_is_clear(PIND, 0)) | 0xE000;

	if (state == 0xF000) 
		return 1;
	
	return 0;
}

void get_digits(struct segment *seg, uint8_t count)
{
	seg->ones = count%10;
	seg->tens = count/10;
}

//*******************************************************************************
// Check switch S0.  When found low for 12 passes of "debounc_switch(), increment
// PORTB.  This will make an incrementing count on the port B LEDS. 
//*******************************************************************************
int main()
{
	DDRB = 0xFF;  //set port B to all outputs

	uint8_t count = 0;

	struct segment seg;

	while (1) {     //do forever
		if (debounce_switch()) {
			PORTB = 0; // zero out portb
		   
			count++; // increment the count
			if (count == 100)
				count = 0;

			get_digits(&seg, count); //get the digits that are to be displayed

			PORTB |= (seg.tens << 4) | (seg.ones); // display the digits
		}

		_delay_ms(2);                    //keep in loop to debounce 24ms
  } //while 
} //main
