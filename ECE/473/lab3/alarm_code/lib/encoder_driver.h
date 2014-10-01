#ifndef ENCODER_DRIVER
#define ENCODER_DRIVER

/******************************************************
 * Extracts bits 0 and 1 from the shift register, which
 * correspond to the left encoder's state.
 ******************************************************/
#define LEFT_ENCODER(byte) (byte & 0x03)

/******************************************************
 * Extracts bits 2 and 3 from the shift register, which
 * correspond to the right encoder's state.
 ******************************************************/
#define RIGHT_ENCODER(byte) ((byte & 0x0C) >> 2)

#define STOPPED 2
#define FORWARD 1
#define REVERSE 0

/******************************************************
 * Holds the last known state of the encoders, which
 * corresponds to the direction it was moving in and
 * the bits that were returned from the shift register.
 ******************************************************/
struct encoder_state {
	uint8_t left_dir;
	uint8_t right_dir;

	uint8_t left_last;
	uint8_t right_last;
};

/******************************************************
 * Enables the encoders by setting up PE6 as the latch
 * for the encoder shift register and enabling the SPI
 * interface.
 ******************************************************/
void enable_encoders();

/******************************************************
 * TODO: think about putting the check on a timer
 ******************************************************/
void init_encoder_timers();

/******************************************************
 * Gets the state of the encoders by initializing an
 * SPI transmission and returning the results after
 * latching the data in the encoder shift register.
 ******************************************************/
char poll_encoders();

/******************************************************
 * Initializes the encoder struct with "STOPPED" as
 * the direction for both of the encoders and starts an
 * SPI transmission to get the current state of the
 * encoders.
 ******************************************************/
void init_encoder_struct(volatile struct encoder_state *state);

/******************************************************
 * Takes an encoder_state struct that represents the
 * last known state of the encoders and determines the
 * current state of the encoders from it.
 ******************************************************/
void get_direction(volatile struct encoder_state *state, uint8_t encoders);

#endif
