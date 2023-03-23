/*
 * APP.h
 *
 *  Created on: Dec 16, 2022
 *      Author: Ahmed
 */

#ifndef APP_APP_H_
#define APP_APP_H_

// Includes
#include "../UTILS/types.h"
#include "../UTILS/MCU.h"
#include "../UTILS/common_macros.h"
#include "../MCAL/DIO/DIO.h"
#include "../MCAL/EXT_INT/interrupt.h"
#include "../MCAL/TIMER/Timer_Interface.h"
#include "../MCAL/FAST_PWM/PWM.h"
#include "../MCAL/UART/UART.h"
#include "../SERVICE/ICU/ICU.h"
#include "../HAL/LCD/LCD.h"
#include "../HAL/Ultrasonic/ultrasonic.h"
#include "../HAL/MOTOR/motor.h"
#include "../HAL/BUTTON/button.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

// defining application devices
// first: input devices
#define BUTTON_INC_PORT		DPORTD
#define BUTTON_INC_PIN		PIN2

#define BUTTON_DEC_PORT		DPORTB
#define BUTTON_DEC_PIN		PIN2

/*#define BUTTON_BRAKE_PORT	DPORTD
#define BUTTON_BRAKE_PIN	PIN3*/

// second: OUTPUT devices
#define MOTOR_PORT			DPORTA
#define MOTOR_PIN			PIN0

#define US_TRIGGER_PORT		DPORTD
#define US_TRIGGER_PIN		PIN4


// MODES DEFINITIOS
#define APP_DEFAULT_MODE		0
#define APP_ULTRASONIC_MODE		1
#define APP_OFF_MODE			2

// Timer config
#define APP_TIMER_CHANNEL			TIMER0
#define APP_TIMER_INTERRUPT_MODE	INTERRUPT
#define APP_TIMER_MODE				TIMER_MODE
#define APP_TIMER_PSC				F_CPU_CLOCK_1024_TIMER_0
#define APP_TIMER_TICKS				TIMER0_OVERFLOW-1
#define APP_TIMER_OVF_MS			16

void APP_init();
void APP_readSensors();
void APP_ticksIncrement(void);
void APP_getCurrentTicks_ms(u64_t *time_ms);
void APP_BTN_INC_readState(void);
void APP_BTN_DEC_readState(void);

#endif /* APP_APP_H_ */
