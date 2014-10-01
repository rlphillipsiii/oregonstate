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
#include <graph_driver.h>

int main()
{     
	uint8_t i; //dummy counter

	enable_graph();  //initalize SPI port

	while (1) {
		for (i = 0; i < NUM_COUNT; i++) {
			update_graph(GRAPH_NUMS[i]);

			_delay_ms(250);
			_delay_ms(250);
		}

		for (i = NUM_COUNT-2; i > 0; i--) {
			update_graph(GRAPH_NUMS[i]);

			_delay_ms(250);
			_delay_ms(250);
		}
	} //while(1)
} //main
