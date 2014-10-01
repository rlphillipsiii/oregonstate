#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi_driver.h"
#include "encoder_driver.h"
#include "macros.h"

#define STROBE_BIT  PE6
#define STROBE_PORT PORTE

void latch_encoders()
{
	strobeh(STROBE_PORT, STROBE_BIT); // latch the encoder state
}

char poll_encoders()
{
	strobeh(STROBE_PORT, STROBE_BIT); // latch the encoder state
	
	return spi_send(0x00); // send garbage in order to receive the state
}

void init_encoder_struct(volatile struct encoder_state *state)
{
	/* set the current encoder directions to stopped */
	state->left_dir = STOPPED;
	state->right_dir = STOPPED;

	/* get the initial value of the encoders in the shift register */
	char encoders = poll_encoders();
	state->left_last = LEFT_ENCODER(encoders);
	state->right_last = RIGHT_ENCODER(encoders);
}						 
				
uint8_t determine_direction(uint8_t cur, uint8_t last)
{
	if (cur == last)
		return STOPPED; // nothing has changed so it's stopped
	
	if (cur == 0x00 && last == 0x01)
		return REVERSE;

	if (cur == 0x01 && last == 0x00)
		return FORWARD;

	return STOPPED;
}

void get_direction(volatile struct encoder_state *state, uint8_t encoders)
{
	/* determine the new direction based on the last and latest encoder state */
	state->left_dir = determine_direction(LEFT_ENCODER(encoders), state->left_last);
	state->right_dir = determine_direction(RIGHT_ENCODER(encoders), state->right_last);

	/* log the last encoder state */
	state->left_last = LEFT_ENCODER(encoders);
	state->right_last = RIGHT_ENCODER(encoders);
}

void enable_encoders()
{
	DDRE |= 0x40; // set PE6 to an output
	
	SETBIT(STROBE_PORT, STROBE_BIT); // set PE6
	spi_init(); // initialize the spi interface
}

