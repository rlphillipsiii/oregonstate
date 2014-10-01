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
#include <encoder_driver.h>
#include <led_driver.h>

int main()
{   
	DDRB = 0xF0;

	struct display output;
	init_display_struct(&output);
	
	uint8_t number = 0;
	uint8_t rnumber = 0;
	uint8_t lnumber = 0;

	uint8_t llast = 0;
	uint8_t rlast = 0;

	enable_leds();
	enable_encoders();

	struct encoder_state encoders;
	init_encoder_struct(&encoders);

	while (1) {
		number = poll_encoders();
		get_direction(&encoders, number);

		if (encoders.left_dir != STOPPED)
			rnumber = (1+encoders.left_dir)*10;

		if (encoders.right_dir != STOPPED)
			lnumber = encoders.right_dir;

		update_segment(&output, rnumber+lnumber);

		_delay_us(1);
	} //while(1)
} //main
