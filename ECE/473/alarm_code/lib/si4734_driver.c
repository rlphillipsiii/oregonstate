//Si4734 i2C functions     
//Roger Traylor 11.13.2011
//device driver for the si4734 chip.

// header files
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdlib.h>
#include <util/twi.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#include "macros.h"
#include "twi_master.h"        //my defines for TWCR_START, STOP, RACK, RNACK, SEND
#include "si4734_driver.h"

#define MAX_FM_FREQ 10790
#define MIN_FM_FREQ  8810

uint8_t si4734_wr_buf[9];          //buffer for holding data to send to the si4734 
uint8_t si4734_rd_buf[15];         //buffer for holding data recieved from the si4734
uint8_t si4734_tune_status_buf[8]; //buffer for holding tune_status data  

enum radio_band{FM, AM, SW};
volatile enum radio_band current_radio_band = FM;

uint16_t current_fm_freq;
uint16_t last_fm_freq;

uint16_t current_am_freq;
uint16_t current_sw_freq;

uint8_t muted;

//********************************************************************************
//                            get_int_status()
//
uint8_t get_int_status()
{
	//send get_int_status command
    si4734_wr_buf[0] = 0x14;
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 1);

	_delay_ms(5);
	//get the interrupt status 
    twi_start_rd(SI4734_ADDRESS, si4734_rd_buf, 1);
    return(si4734_rd_buf[0]);
}

//********************************************************************************

//********************************************************************************
//                            fm_tune_freq()
//
//takes current_fm_freq and sends it to the radio chip
void fm_tune_freq(uint8_t wait)
{
	if (current_fm_freq == last_fm_freq)
		return;
	
	si4734_wr_buf[0] = 0x20;  //fm tune command
	si4734_wr_buf[1] = 0x00;  //no FREEZE and no FAST tune
	si4734_wr_buf[2] = (uint8_t)(current_fm_freq >> 8); //freq high byte
	si4734_wr_buf[3] = (uint8_t)(current_fm_freq);      //freq low byte
	si4734_wr_buf[4] = 0x00;  //antenna tuning capactior

	//send fm tune command
	twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 5);
	last_fm_freq = current_fm_freq;

	if (wait)
		_delay_ms(80);
}

uint16_t get_freq()
{
	return current_fm_freq/10;
}

void fm_alter_freq(uint8_t inc)
{
	if (inc) {
		current_fm_freq += 20;
		if (current_fm_freq == 10810)
			current_fm_freq = MIN_FM_FREQ;
	} else {
		current_fm_freq -= 20;
		if (current_fm_freq == 8790)
			current_fm_freq = MAX_FM_FREQ;
	}
}

//********************************************************************************

//********************************************************************************
//                            am_tune_freq()
//
//takes current_am_freq and sends it to the radio chip
//
void am_tune_freq()
{
	si4734_wr_buf[0] = 0x40;  //am tune command
	si4734_wr_buf[1] = 0x00;  //no FAST tune
	si4734_wr_buf[2] = (uint8_t)(current_am_freq >> 8); //freq high byte
	si4734_wr_buf[3] = (uint8_t)(current_am_freq);      //freq low byte
	si4734_wr_buf[4] = 0x00;  //antenna tuning capactior high byte
	si4734_wr_buf[5] = 0x00;  //antenna tuning capactior low byte

	//send fm tune command
	twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 6);

    _delay_ms(80);
}

//********************************************************************************

//********************************************************************************
//                            sw_tune_freq()
//
//takes current_sw_freq and sends it to the radio chip
//antcap low byte is 0x01 as per datasheet
void sw_tune_freq()
{
	si4734_wr_buf[0] = 0x40;  //am tune command
	si4734_wr_buf[1] = 0x00;  //no FAST tune
	si4734_wr_buf[2] = (uint8_t)(current_sw_freq >> 8); //freq high byte
	si4734_wr_buf[3] = (uint8_t)(current_sw_freq);      //freq low byte
	si4734_wr_buf[4] = 0x00;  //antenna tuning capactior high byte
	si4734_wr_buf[5] = 0x01;  //antenna tuning capactior low byte 

	//send am tune command
	twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 6);

    _delay_ms(80);
}

//********************************************************************************
//                            fm_pwr_up()
//
void fm_pwr_up()
{
	//send fm power up command
	si4734_wr_buf[0] = 0x01;
	si4734_wr_buf[1] = 0x50; //GPO2OEN and XOSCEN selected
	si4734_wr_buf[2] = 0x05; //analog audio outputs
	twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 3);

	_delay_ms(120);   //startup delay as specified 
  
	set_property(GPO_IEN, (1<<GPO_IEN_STCIEN_SHFT), 1);    //enable Seek/Tune Complete interrupt
}

//********************************************************************************
//                            am_pwr_up()
//
void am_pwr_up()
{
	//send am power up command
	si4734_wr_buf[0] = 0x01;
	si4734_wr_buf[1] = 0x51;//GPO2OEN and XOSCEN selected
	si4734_wr_buf[2] = 0x05;
	twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 3);

	_delay_ms(120);   

	set_property(GPO_IEN, (1<<GPO_IEN_STCIEN_SHFT), 1);    //enable Seek/Tune Complete interrupt
}
//********************************************************************************

//********************************************************************************
//                            sw_pwr_up()
//
void sw_pwr_up()
{
	//send sw power up command (same as am, only tuning rate is different)
    si4734_wr_buf[0] = 0x01;
    si4734_wr_buf[1] = 0x51;
    si4734_wr_buf[2] = 0x05;
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 3);

	//get the interrupt status 
	_delay_ms(120);   

	//set property to disable soft muting for shortwave broadcasts
	set_property(AM_SOFT_MUTE_MAX_ATTENUATION, 0x0000, 1); //cut off soft mute  
	//select 4khz filter BW and engage power line filter
	set_property(AM_CHANNEL_FILTER, (AM_CHFILT_4KHZ | AM_PWR_LINE_NOISE_REJT_FILTER), 1); 
	set_property(GPO_IEN, (1<<GPO_IEN_STCIEN_SHFT), 1);    
}

//********************************************************************************

//********************************************************************************
//                            radio_pwr_dwn()
//
void radio_pwr_dwn()
{
	//send fm power down command
    si4734_wr_buf[0] = 0x11;
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 1);

	_delay_us(310);
}
//********************************************************************************

//********************************************************************************
//                            fm_rsq_status()
//
void fm_rsq_status(uint8_t wait)
{
    si4734_wr_buf[0] = FM_RSQ_STATUS;            //fm_rsq_status command
    si4734_wr_buf[1] = FM_RSQ_STATUS_IN_INTACK;  //clear STCINT bit if set
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 2);

	if (wait)
		_delay_ms(5);

	//get the fm tune status 
    twi_start_rd(SI4734_ADDRESS, si4734_tune_status_buf, 8);
}

void get_signal_strength(char signal[11])
{
	uint8_t strength = si4734_tune_status_buf[3];
	sprintf(signal, "%03d/255   ", strength);
}

//********************************************************************************
//                            fm_tune_status()
//
void fm_tune_status()
{
    si4734_wr_buf[0] = FM_TUNE_STATUS;            //fm_tune_status command
    si4734_wr_buf[1] = FM_TUNE_STATUS_IN_INTACK;  //clear STCINT bit if set
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 2);
    _delay_ms(5);

	//get the fm tune status 
    twi_start_rd(SI4734_ADDRESS, si4734_tune_status_buf, 8);
}

//********************************************************************************
//                            am_tune_status()
//
//TODO: could probably just have one tune_status() function
void am_tune_status()
{
    si4734_wr_buf[0] = AM_TUNE_STATUS;            //fm_tune_status command
    si4734_wr_buf[1] = AM_TUNE_STATUS_IN_INTACK;  //clear STCINT bit if set
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 2);
    _delay_ms(5);

	//get the am tune status 
    twi_start_rd(SI4734_ADDRESS, si4734_tune_status_buf, 8);

}

//********************************************************************************
//                            am_rsq_status()
//
void am_rsq_status()
{
    si4734_wr_buf[0] = AM_RSQ_STATUS;            //am_rsq_status command
    si4734_wr_buf[1] = AM_RSQ_STATUS_IN_INTACK;  //clear STCINT bit if set
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 2);
    _delay_ms(5);

	//get the fm tune status 
    twi_start_rd(SI4734_ADDRESS, si4734_tune_status_buf, 8);
}

//********************************************************************************
//                            set_property()
//
void set_property(uint16_t property, uint16_t property_value, uint8_t wait)
{
    si4734_wr_buf[0] = SET_PROPERTY;                   //set property command
    si4734_wr_buf[1] = 0x00;                           //all zeros
    si4734_wr_buf[2] = (uint8_t)(property >> 8);       //property high byte
    si4734_wr_buf[3] = (uint8_t)(property);            //property low byte
    si4734_wr_buf[4] = (uint8_t)(property_value >> 8); //property value high byte
    si4734_wr_buf[5] = (uint8_t)(property_value);      //property value low byte
    twi_start_wr(SI4734_ADDRESS, si4734_wr_buf, 6);

	if (wait)
		_delay_ms(10);  //set properties takes 10ms to complete
}//set_property()

void reset_radio()
{
	PORTE &= ~(1 << PE7); //int2 initially low to sense TWI mode
	DDRE  |= 0x80;

	PORTE |=  (1 << PE2); //hardware reset Si4734 
	_delay_us(200);     //hold for 200us, 100us by spec         
	PORTE &= ~(1 << PE2); //release reset 
	_delay_us(30);      //5us required because of my slow I2C translators I suspect

	DDRE  &= ~(0x80);   //now Port E bit 7 becomes input from the radio interrupt
}

void mute_radio(uint8_t wait)
{
	muted = TRUE;
	set_property(RX_HARD_MUTE, MUTE_LR, wait);
}

void unmute_radio()
{
	muted = FALSE;
	set_property(RX_HARD_MUTE, UNMUTE, 0);
}

uint8_t is_muted()
{
	return muted;
}

void enable_radio()
{
	current_fm_freq = 9910;
	last_fm_freq = 0;

	reset_radio();
	init_twi();

	fm_pwr_up();
	
	mute_radio(1);
	fm_tune_freq(1);
}
