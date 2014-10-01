// lm73_functions.c       
// Roger Traylor 11.28.10

#include <util/twi.h>
#include "lm73_functions.h"
#include <util/delay.h>

//TODO: remove volatile type modifier?  I think so.
//TODO: initalize with more resolution and disable the smb bus timeout
//TODO: write functions to change resolution, alarm etc.

volatile uint8_t lm73_wr_buf[2];
volatile uint8_t lm73_rd_buf[2];

//********************************************************************************

//******************************************************************************
uint8_t lm73_temp_convert(char temp_digits[], uint16_t lm73_temp, uint8_t f_not_c){
	//given a temperature reading from an LM73, the address of a buffer
	//array, and a format (deg F or C) it formats the temperature into ascii in 
	//the buffer pointed to by the arguement.
	//TODO:Returns what???(uint8_t)??? Probably a BUG?
	uint8_t  round_up;    //boolean, round up temperature digits or not 
	uint16_t mask;        //sliding one bit for multiplcation
	uint8_t  minus_temp;  //boolean, indicates a negative celcius temperautre
	uint32_t lsb_value;   //holds value of the lsb bit of the temperature reading
	uint8_t  i;           //index  
	uint32_t temp_fract;  //a fractional (parital) part of the temperature

	//incoming data from LM73 formatted as follows:
	//bit     15   14  13  12  11  10   9   8   7    6    5    4     3      2  1   0
	//Deg C  +/-  128  64  32  16   8   4   2   1   .5  .25 .125 .0625 .03125  X   X

    if(lm73_temp & 0x8000) {minus_temp = TRUE; }  //get info on sign bit
    else                   {minus_temp = FALSE;}
    lm73_temp = (lm73_temp << 1);               //shift off the sign bit

    if(minus_temp == TRUE){ //take 2's compliment, always deal with positive temps
		lm73_temp ^= 0xFFFF;   //invert all bits 
		lm73_temp += 0x08;     //add in one lsb
    }

	//now, lm73_temp looks like:
	//bit     15   14  13  12  11  10   9   8    7    6    5     4      3  2  1  0
	//Deg C  128   64  32  16   8   4   2   1   .5  .25 .125 .0625 .03125  X  X  0
	//mask                                       0    0    0     0      1

    if(f_not_c == TRUE){lsb_value = 5625;} // (9/5) * 3125
    else                {lsb_value = 3125;} //.03125 deg Celcius
    
    //sum up the total temperature by adding multiples of the multiplied lsb 
    mask = 0x0008;           //start summing in bit position 3 (lsb)
    temp_fract = 0;          //initalize the sum
    for(i=0; i<=12; i++){    //sum for each fractional bit set in temperature
		if(lm73_temp & mask){temp_fract += (lsb_value << i);}
		mask = (mask << 1);    //shift mask to next bit
    }//for

    if((temp_fract % 1000) > 500) {round_up = TRUE;}  //round up .01deg?
    temp_fract = temp_fract/1000;    //gives two decimal places for temperature
    if(round_up){temp_fract += 1;}   //add a 1/100 if round up was true
    if(f_not_c == TRUE){
		if (minus_temp == FALSE){temp_fract += 3200;}  //32 degree offset for Deg F
		else                    {temp_fract = 3200 - temp_fract;} 
		//some other cases to take into account here  BUG
    }
    
    //extract each digit and put in the temperature buffer array
    for(i=0; i<=5; i++) {    //upto 5 numeric digits possible plus a decimal point
		if(i==2){continue;}    //skip when its the decimal points place [2]
		temp_digits[i] = temp_fract % 10;         //gets lowest digit
		temp_fract = temp_fract - temp_digits[i]; //subsract away that digit
		temp_fract = temp_fract / 10;                //throw away lowest digit
    }
   
    //convert decimal digits to ascii values, [2] is a decimal point
    temp_digits[0] += 0x30; 
    temp_digits[1] += 0x30; 
    temp_digits[2] =  0x2E; 
    temp_digits[3] += 0x30; 
    temp_digits[4] += 0x30; 
    temp_digits[5] += 0x30; 

    if(temp_digits[5] == 0x30){temp_digits[5] = 0x20;} //remove leading zero
    //could have two leading zero digits!  BUG
    if(minus_temp){temp_digits[5] = 0x2D;}                  //minus sign, cold!
    //could have two leading zero digits!
 
	return(0);
}//lm73_temp_convert
//******************************************************************************
