/*
 * Timer0_pwm.h
 *
 *  Created on: Sep 14, 2022
 *      Author: Clara Isaac
 */

#ifndef TIMER0_PWM_H_
#define TIMER0_PWM_H_

#include <avr/io.h>
#include "std_types.h"
/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/* Description :
 *1. The function responsible for trigger the Timer0 with the PWM Mode.
 *2. Setup the PWM mode with Non-Inverting.
 *3. Setup the prescaler with F_CPU/8.
 *4. Setup the compare value based on the required input duty cycle
 *5. Setup the direction for OC0 as output pin through the GPIO driver.
 *6. The generated PWM signal frequency will be 500Hz to control the DC Motor speed.
 */

void Timer0_pwm_Start(uint8 duty_cycle);

#endif /* TIMER0_PWM_H_ */
