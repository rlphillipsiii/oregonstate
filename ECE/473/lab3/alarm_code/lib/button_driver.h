#ifndef BUTTON_DRIVER
#define BUTTON_DRIVER

/******************************************************
 * Enables the buttons to be read on PORTA.  This 
 * will also disable the LED display.
 ******************************************************/
void enable_buttons();

/******************************************************
 * Checks whether or not a button has been pressed
 * using a software debouncing algorithm.
 ******************************************************/
uint8_t debounce_switch(uint16_t *state, uint8_t bit);

/******************************************************
 * Abstracts away the software debouncing by returning
 * a byte with each bit representing the state of
 * PA7-PA0 respectively.  The byte is encoded as if
 * the buttons were active high.
 ******************************************************/
char poll_buttons(char reg);

#endif
