/*
 * DC_motor.c
 *
 *  Created on: Sep 14, 2022
 *      Author: Clara Isaac
 */

#include "GPIO.h"
#include "Timer0_pwm.h" /* To use the PWM Function */
#include "DC_motor.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the DC motor:
 * 1. Setup the DC motor pins directions by use the GPIO driver.
 * 2. Initialize Values on pins.
 */
void DC_motor_init(void)
{
	/* Configure the direction for A, B pins as output pins */
	GPIO_setupPinDirection(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, PIN_OUTPUT);

	/* Write on the PINS Logic Low */
	GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_LOW);
	GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_LOW);

}

/*
 * Description:
 * Move the motor with a specific speed using PWM timer0
 */
void speed_of_DC_motor(uint8 temp)
{
	if (temp <= 19) /* if temperature less than or equal to 19, turn off the motor */
	{
		GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_LOW);
		return;
	}
	/* more than 19 degree, turn on the motor */
	else if (DIRECTION) /* Clockwise Direction */
	{
		GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_HIGH);
		GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_LOW);
	}
	else /* Anti-clockwise Direction */
	{
		GPIO_writePin(DC_motor_A_PORT_ID, DC_motor_A_PIN_ID, LOGIC_LOW);
		GPIO_writePin(DC_motor_B_PORT_ID, DC_motor_B_PIN_ID, LOGIC_HIGH);
	}

	/* max speed occurs at temperature = 40 degree */
	if (temp >= 40)
		temp = 40;
	uint8 duty_cycle = (uint8)((uint16)((temp - 19) * 255) / 21); /* Take the percentage speed and multiply by (2^8 - 1) */
	Timer0_pwm_Start(duty_cycle); /* Send the duty cycle to PWM function to generate the PWM signal */
}

