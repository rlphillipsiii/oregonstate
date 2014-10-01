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

#define F_CPU 16000000UL //16Mhz clock
#include <avr/io.h>
#include <util/delay.h>

//***********************************************************************
//                            spi_init                               
// Initalizes the SPI port on the mega128. Does not do any further   
// external device specific initalizations.  Sets up SPI to be:                        
// master mode, clock=clk/2, cycle half phase, low polarity, MSB first
// interrupts disabled, poll SPIF bit in SPSR to check xmit completion
//**********************************************************************
void spi_init(void){
	DDRB = 0x07;  //set output mode for SS, MOSI, SCLK
	SPCR = (1 << SPE) | (1 << MSTR); //set master mode, clk low on idle, leading edge sample
	SPSR = (1 << SPI2X);  //set choose double speed operation
 }//spi_init


//**********************************************************************
//                                main                                 
//**********************************************************************
int main(){     

	uint8_t display_count = 0x01; //holds count for display 
	uint8_t i; //dummy counter

	//do initialization
	spi_init();  //initalize SPI port
	DDRD = 0xFF;
	PORTD = 0;

	while(1){                             //main while loop
		SPDR = display_count;             //send to display 
		while (bit_is_clear(SPSR,SPIF)){} //wait till data sent out
		PORTD |= (1 << PD2);                   //rising edge to regclk on HC595 
		PORTD &= ~(1 << PD2);                   //falling edge

		display_count = (display_count << 1);        //shift for next time 
		display_count |= 1;

		if(display_count==0xFF){display_count=0x01;} //display back to 1st positon
		for(i=0; i<=4; i++){_delay_ms(100);}         //0.5 sec delay
  } //while(1)
} //main
