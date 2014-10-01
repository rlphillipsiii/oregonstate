#ifndef SPI_DRIVER
#define SPI_DRIVER

/******************************************************
 * Sets the given bit on the given port and the
 * immediately clears it to give a pulse.
 ******************************************************/
#define strobel(port, bit) (port |= (1 << bit)); (port &= ~(1 << bit));

/******************************************************
 * Clears the given bit on the given port and then
 * immediately sets it to give a pulse.
 ******************************************************/
#define strobeh(port, bit) (port &= ~(1 << bit)); (port |= (1 << bit));

/******************************************************
 * Enables the SPI interface.
 ******************************************************/
void spi_init();

void spi_wait();

/******************************************************
 * Initiates an SPI transmission by writing the given
 * byte to SPDR and then waiting for the transmission
 * to finish.  The contents of SPDR are then returned.
 ******************************************************/
char spi_send(char msg);

#endif