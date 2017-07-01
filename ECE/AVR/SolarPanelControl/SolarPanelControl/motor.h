/** 
 * @file
 * @author Robert L. Phillips III
 * @version 1.1
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * @section DESCRIPTION
 * 
 * Includes functions for setting up PWM on PB5 and controlling the motor
 * that is to have a solar panel mounted on it
 * 
 * @section HARDWARE
 * Target Device: ATTINY261
 */

#ifndef MOTOR_H_
#define MOTOR_H_

/** 
 * The Tiny261 CPU speed
 */
#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#include "adc.h"


/**
 * Calculates the absolute value of the given number
 */
#define ABS(x) ((x < 0) ? -x : x)

/**
 * Lower bound for the motor position when the Tiny looks for the
 * position where the solar panel receives the most light
 */
#define LOWER_BOUND 0

/**
 * Upper bound for the motor position when the Tiny looks for the
 * position where the solar panel receives the most light
 */
#define UPPER_BOUND 255

/** 
 * The motor forward direction
 */
#define FORWARD 1

/** 
 * The motor reverse direction
 */
#define REVERSE 0

/** 
 * The motor stop moving command
 */
#define STOP 0

/** 
 * The motor counter-clockwise motion command
 */
#define CCW 1

/** 
 * The motor clockwise motion command
 */
#define CW 2

/** 
 * The default duty cycle for the PWM on PB5, which determines the motor
 * speed
 */
#define SERVO_SPEED 175

/** 
 * The max error for the motor position that is considered acceptable
 */
#define DEADBAND 15

/** 
 * The time the motor is allowed to correct its position before it stops
 */
#define SERVO_POSITION_TIMEOUT 1000

/** 
 * The duty cycle for PWM on PB5 (OCR1D)
 */
#define PWMD OCR1D

/** 
 * The TOP of the PWM channel (OCR1C)
 */
#define TOP OCR1C

/** 
 * The potentiometers ADC channel
 */
#define POT_ADC 5

/** 
 * The solar panels ADC channel
 */
#define SOLAR_ADC 6


/** 
 * Sets the motor direction to spin in the forward direction
 */
void set_motor_forward();

/** 
 * Sets the motor direction to spin in the reverse direction
 */
void set_motor_reverse();

/** 
 * Sets the motor to stop
 */
void set_motor_stop();

/** 
 * Sets the motor direction to spin in the supplied direction
 *
 * @param direction The direction that the motor is to spin in
 *
 * @see FORWARD
 * @see REVERSE
 */
void set_direction(int direction);

/** 
 * Moves the motor in the supplied direction or stops the motor if
 * the stop command is given
 *
 * @param dir The direction that the motor is to move in
 *
 * @see CW
 * @see CCW
 * @see STOP
 * @see PWMD
 * @see SERVO_SPEED
 */
void move_motor(int dir);

/** 
 * Gets the error in the motor position by comparing the current position
 * of the motor to the destination that is supplied
 *
 * @param destination The destination (between 0 and 255) that the servo motor
 * is to move
 *
 * @return The difference between the destination and current motor position
 *
 * @see ADC_REF_VCC
 * @see POT_ADC
 * @see UPPER_BOUND
 * @see LOWER_BOUND
 */
int get_error(uint8_t destination);

/** 
 * Determines which direction the motor needs to spin in order for the motor
 * to get closer to its destination and moves the motor accordingly
 *
 * @param destination The desired destination (between 0 and 255) of the motor
 *
 * @see DEADBAND
 */
void update_servo_position(uint8_t destination);

/** 
 * Enables fast PWM on PB5
 */
void enable_PWM();

/** 
 * Moves the motor as close to the desired destination that it can get before
 * the timeout expires
 *
 * If 0 is given as the destination, the motor will retain its current position.  
 * This is meant to be used when the computer wants to get the solar panel
 * measurements without moving the motor.
 *
 * @param destination The desired destination (between 0 and 255)
 * @param seekTime The max time the motor can take to reach its destination
 *
 * @see SERVO_POSITION_TIMEOUT
 */
void set_servo_position(uint8_t destination, uint16_t seekTime);



#endif /* MOTOR_H_ */