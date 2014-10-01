#ifndef ADC_DRIVER
#define ADC_DRIVER

/******************************************************
 * Enables the adc to be read from.  The voltage for
 * the adc should be applied to PF7, and is compared
 * to the external 5V source.
 ******************************************************/
void enable_adc();

/******************************************************
 * Starts an adc conversion.  This function does NOT
 * wait for the adc to be finished.
 ******************************************************/
void start_adc();

/******************************************************
 * Starts an adc conversion, waits for it to finish
 * and then returns a 10 bit value correspsonding to
 * the adc measure value.
 ******************************************************/
uint16_t poll_adc();

/******************************************************
 * Checks to see if the adc is running.
 ******************************************************/
uint8_t is_adc_running();

/******************************************************
 * Returns the 10 bit value measured by the adc.
 ******************************************************/
uint16_t get_adc();

/******************************************************
 * Maps the 10 bit adc value to an 8 bit value.
 ******************************************************/
uint8_t get_adc_adjusted(uint16_t raw);

#endif
