#ifndef GRAPH_DRIVER
#define GRAPH_DRIVER

#define OFF   0x00
#define ONE   0x80
#define TWO   0xC0
#define THREE 0xE0
#define FOUR  0xF0
#define FIVE  0xF8
#define SIX   0xFC
#define SEVEN 0xFE
#define EIGHT 0xFF

#define NUM_COUNT 9

uint8_t GRAPH_NUMS[] = {OFF, ONE, TWO, THREE, FOUR, 
					  FIVE, SIX, SEVEN, EIGHT};

/******************************************************
 * Enables the graph by setting up PD0 as the latch
 * for the bar graph display and enabling the SPI
 * interface.
 ******************************************************/
void enable_graph();

/******************************************************
 * Updates the bar graph with the appropriate value by
 * initiating an SPI transmission with given value and
 * telling the bar graph to latch the new data when the
 * transmission completes.
 ******************************************************/
char update_graph(char value);

#endif
