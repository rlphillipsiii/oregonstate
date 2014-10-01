// bar_graph_simple_skel.c 
// R. Traylor
// 10.22.13
// demos SPI

// Every half second, a new data led is lit on the bargraph display via SPI
// in a climbing pattern. 

// Expected Connections:
// Bargraph board            mega128
// --------------       --------------------     
//     reglck           PORTB bit 0 (SS_n)                       
//     srclk            PORTB bit 1 (sclk)
//     sdin             PORTB bit 2 (mosi)
//     oe_n             ground (short to gnd2)
//     gnd2             ground
//     vdd2             vcc
//     sd_out           no connect
#define F_CPU 16000000L //16Mhz clock

#include <avr/io.h>
#include <util/delay.h>
#include <adc_driver.h>
#include <led_driver.h>
#include <macros.h>

int main()
{   
	DDRB = 0xF0;

	TCCR2 = (1 << WGM20) | (1 << WGM21) | (1 << COM21) | (1 << COM20) | (1 << CS22);
	OCR2 = 10;

	struct display output;
	init_display_struct(&output);
	update_colon(&output, 1, 0);
	
	uint16_t number = 0;
 
	enable_leds();
	enable_adc();

	number = poll_adc();
	start_adc();
	
	while (1) {
		if (!is_adc_running()) {
			update_segment(&output, number);
			SETBIT(ADCSRA, ADIF);
			number = get_adc();

			OCR2 = get_adc_adjusted(number);
			start_adc();
		}

		update_segment(&output, number);
	} //while(1)
} //main
