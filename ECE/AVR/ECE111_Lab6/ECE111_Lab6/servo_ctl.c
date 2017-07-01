/** 
 * The Tiny261 CPU speed
 */
#define F_CPU 1000000UL

#include <util/delay.h>
#include <avr/io.h>

#include "adc.h"
#include "motor_ctl.h"
#include "servo_ctl.h"

void SetMotorDirection(int dir)
{
	if (dir == CCW)
	{
		PWMB = SERVO_SPEED;
		motorB_Forward();
	}
	else if (dir == CW)
	{
		PWMB = SERVO_SPEED;
		motorB_Reverse();
	}
	else 
	{
		PWMB = 0;
		motorB_Stop();
	}
}


int ServoError(uint8_t destination)
{
	uint8_t position;
	
	ADCSetReference(ADC_REF_VCC);
	position = ReadADC(POT_ADC);
	
	if (destination > UPPER_BOUND)
	{
		destination = UPPER_BOUND;	// Set it to the upper bound
	}
	else if (destination < LOWER_BOUND)
	{
		destination = LOWER_BOUND;	// Set it to the lower bound
	}
		
	return position - destination;
}


void UpdateServo(uint8_t destination)
{
	int error = ServoError(destination);
	
	if (ABS(error) < DEADBAND)	// If the error is less than the deadband, just turn off the motor.
	{
		SetMotorDirection(STOP);
	}
	else if (error > 0)	// If the error is negativ, turn the servo clockwise.
	{
		SetMotorDirection(CCW);
	}
	else	// Otherwise turn the servo counter-clockwise.
	{
		SetMotorDirection(CW);
	}
}


void setServoPosition(uint8_t destination, uint16_t seekTime)
{
	uint16_t i;
	for (i = 0; i < seekTime; i++)
	{
		UpdateServo(destination);
		_delay_ms(1);
	}
}