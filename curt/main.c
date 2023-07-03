/*
 * main.c
 *
 *  Created on: Sep 14, 2022
 *      Author: Clara Isaac
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "common_macros.h"
#include "GPIO.h"
#include "std_types.h"
#include "lm35.h"
#include "DC_motor.h"
#include "Timer0_pwm.h"
#include "adc.h"
#include "LCD.h"

/* Ports and Pins of LEDs, Buzzer and Push Button */
#define GREEN_LED_PORT PORTB_ID
#define YELLOW_LED_PORT PORTB_ID
#define RED_LED_PORT PORTB_ID
#define BUZZER_PORT PORTB_ID
#define PUSH_BOTTON_PORT PORTD_ID

#define GREEN_LED_PIN PIN2_ID
#define YELLOW_LED_PIN PIN1_ID
#define RED_LED_PIN PIN0_ID
#define BUZZER_PIN PIN6_ID
#define PUSH_BOTTON_PIN PIN2_ID

/* Variables Declaration */
uint8 flag = 1; // To indicate If the temperature didn't exceed the 40 degree
uint8 temperature; // variable to store the temperature

/* Interrupt Service Routine */
ISR(INT0_vect)
{
	if (temperature < 40)
	{
		flag = 1;
		GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
		LCD_clearScreen();
	}
}

/* Interrupt Initialization */
void INT0_Init(void)
{
	GPIO_setupPinDirection(PUSH_BOTTON_PORT, PUSH_BOTTON_PIN, PIN_INPUT); // set push button pin as input pin
	MCUCR |= (1<<ISC00) | (1<<ISC01);   // Trigger INT0 with the raising edge
	GICR  |= (1<<INT0);// Enable external interrupt pin INT0
	SREG |= (1<<7); // Enable the Global Interrupt (I-bit)
}

int main(void)
{
	/* Specify HW ports and pins directions */
	GPIO_setupPinDirection(GREEN_LED_PORT, GREEN_LED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(YELLOW_LED_PORT, YELLOW_LED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(RED_LED_PORT, RED_LED_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	/* Peripherals Initialization */
	INT0_Init();
	DC_motor_init();
	LCD_init();

	// ADC For Temp Sensor Configuration
	ADC_ConfigType ADC_Config = {INTERNAL_VOLTAGE, F_CPU_8};
	ADC_init (&ADC_Config);

	/* Initial Values */
	speed_of_DC_motor(20);
	GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
	GPIO_writePin(YELLOW_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
	GPIO_writePin(RED_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);

	while(1)
	{
		/* Prepare The LCD*/
		LCD_moveCursor(0,0);
		LCD_displayString("Temp =      C");
		LCD_moveCursor(0,8);

		/* Get the temperature from the sensor */
		temperature = LM35_getTemperature();

		/* Print the temperature on the LCD */
		if(temperature >= 100)
		{
			LCD_intgerToString(temperature);
		}
		else
		{
			LCD_intgerToString(temperature);
			/* In case the digital value is two or one digits print space in the next digit place */
			LCD_displayCharacter(' ');
		}


		if (flag == 1)
		{
			if (temperature < 20)
			{
				/* Illuminate green LED only and stops the fan if it works */
				GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_HIGH);
				GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
				GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
				//speed_of_DC_motor(temperature);
			}
			else if (temperature >= 20 && temperature < 40)
			{
				/* Illuminate yellow LED only and starts the fan with speed depends on the temperature */
				GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_HIGH);
				GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
				GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_LOW);
				//speed_of_DC_motor(temperature);
			}
			else if (temperature >= 40)
			{
				/* Illuminate red LED only
				 * Put the fan on the highest speed
				 * open the buzzer
				 * display on the LCD "warning"
				 */
				GPIO_writePin(RED_LED_PORT, RED_LED_PIN, LOGIC_HIGH);
				GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
				GPIO_writePin(GREEN_LED_PORT, GREEN_LED_PIN, LOGIC_LOW);
				GPIO_writePin(YELLOW_LED_PORT, YELLOW_LED_PIN, LOGIC_LOW);
				//speed_of_DC_motor(temperature);
				flag = 0;
				LCD_displayStringRowColumn(1,0," WARNING!! ");
			}
			speed_of_DC_motor(temperature);
		}
	}
}
