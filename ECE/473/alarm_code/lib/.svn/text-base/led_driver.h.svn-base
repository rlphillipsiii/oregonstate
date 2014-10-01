#ifndef LED_DRIVER
#define LED_DRIVER

#define COLON_OFF 1
#define COLON_ON  0

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
	uint8_t decimal;
};

void set_zero_blanked(volatile struct display *output);
void set_decimal(volatile struct display *output, uint8_t boolean);

/******************************************************
 * Turns the LED screen off completely.
 ******************************************************/
void disable_leds();

/******************************************************
 * Toggles the alarm armed indicator.
 ******************************************************/
void toggle_dot(volatile struct display *output);

/******************************************************
 * Sets a flag that indicates whether or not the colon
 * should be lit and whether or not the alarm armed
 * indicator should be lit.
 ******************************************************/
void update_colon(volatile struct display *output, uint8_t off, uint8_t all);

/******************************************************
 * Sets a flag that indicates whether or not the
 * display should be zero filled or not.
 ******************************************************/
void set_zero_filled(volatile struct display *output);

/******************************************************
 * Enables the dimmer by enabling the ADC on PF7 and
 * fast PWM on PB7 using TCNT2
 ******************************************************/
void enable_dimmer();

/******************************************************
 * Updates the duty cycle of the PWM on PB7 (TNCT2) to
 * reflect the brightness measured by the adc on PF7.
 ******************************************************/
void update_brightness();

/******************************************************
 * Initializes a display struct by setting all of the
 * digits and the segment to be turned on to 0.
 ******************************************************/
void init_display_struct(volatile struct display *output);

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
void decode_number(volatile struct display *output, uint16_t number);

/******************************************************
 * Sets PORTA to the binary that corresponds to the
 * supplied digit. 
 ******************************************************/
void set_digit(uint8_t digit, uint8_t decimal);

/******************************************************
 * Updates the LED display based on the information
 * in the display struct and the given number.  This
 * function abstracts away all interaction with the
 * display.
 ******************************************************/
void update_segment(volatile struct display *output, uint16_t number);

#endif
