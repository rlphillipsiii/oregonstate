#ifndef LED_DRIVER
#define LED_DRIVER

/******************************************************
 * Struct holding the digits and the segment for the
 * LCD display.
 ******************************************************/
struct display {
	uint8_t ones;
	uint8_t tens;
	uint8_t hundreds;
	uint8_t thousands;

	uint8_t colon;
	uint8_t mode;
	uint8_t cycle;
};

void disable_leds();
void update_colon(struct display *output, uint8_t on, uint8_t all);
void set_zero_filled(struct display *output);

/******************************************************
 * Initializes a display struct by setting all of the
 * digits and the segment to be turned on to 0.
 ******************************************************/
void init_display_struct(struct display *output);

/******************************************************
 * Enables the LED output by setting PORTA to be all
 * outputs.  The push buttons on PORTA will become
 * disabled by this function.
 ******************************************************/
void enable_leds();

/******************************************************
 * Extracts the ones, tens, hundreds and thousands
 * places from the given number.  The extracted values
 * are placed in the supplied struct.
 ******************************************************/
void decode_number(struct display *output, uint16_t number);

/******************************************************
 * Sets PORTA to the binary that corresponds to the
 * supplied digit. 
 ******************************************************/
void set_digit(uint8_t digit);

/******************************************************
 * Updates the LED display based on the information
 * in the display struct and the given number.  This
 * function abstracts away all interaction with the
 * display.
 ******************************************************/
void update_segment(struct display *output, uint16_t number);

#endif
