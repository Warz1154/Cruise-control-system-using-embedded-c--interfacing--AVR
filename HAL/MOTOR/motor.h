/*
 * motor.h
 *
 *  Created on: Dec 16, 2022
 *      Author: Abdelrahman
 */

#include "../../UTILS/types.h"
#include "../../UTILS/MCU.h"
#include "../../UTILS/common_macros.h"
#include "../../MCAL/DIO/DIO.h"
#include "../../MCAL/FAST_PWM/PWM.h"
#include "../../MCAL/EXT_INT/interrupt.h"

#ifndef HAL_MOTOR_MOTOR_H_
#define HAL_MOTOR_MOTOR_H_

#define MOTOR1_PORT		PORTA
#define MOTOR1_PIN		0


typedef enum{
	ON,
	OFF
}state;

typedef struct
{
	state motorstate;
	DIO_PORT_ID PORT;
	DIO_PIN_ID PIN;
}motor_config;

// dont use
void motor1_on();

// dont use
void motor1_off();

void motor_init(motor_config* config);

void motor_set_state(motor_config* config);

void motor_increase_speed();

void motor_increase_speed_by(u8_t percentage);

void motor_decrease_speed_by(u8_t percentage);

void motor_set_speed(u8_t percentage);

void motor_decrease_speed();


#endif /* HAL_MOTOR_MOTOR_H_ */
