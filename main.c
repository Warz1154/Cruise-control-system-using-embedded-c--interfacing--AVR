/*
 * main.c
 *
 *  Created on: Dec 14, 2022
 *      Author: Ahmed
 */


#include "UTILS/types.h"
#include "UTILS/MCU.h"
#include "UTILS/common_macros.h"
#include "MCAL/DIO/DIO.h"
#include "MCAL/EXT_INT/interrupt.h"
#include "MCAL/TIMER/Timer_Interface.h"
#include "MCAL/FAST_PWM/PWM.h"
#include "MCAL/UART/UART.h"
#include "SERVICE/ICU/ICU.h"
#include "HAL/LCD/LCD.h"
#include "HAL/Ultrasonic/ultrasonic.h"
#include "HAL/MOTOR/motor.h"
#include "HAL/BUTTON/button.h"
#include "APP/APP.h"
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

u8_t motor_speed;

int main()
{
	APP_init();
	//GLOBALInt_disbable();
	while(1)
	{
		APP_readSensors();
	}
	return 0;
}
