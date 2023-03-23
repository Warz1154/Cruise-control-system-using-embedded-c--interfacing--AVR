
#include "../BUTTON/button.h"
#include "motor.h"

extern u8_t motor_speed;

void motor1_on(void)
{
	SET_BIT(MOTOR1_PORT, MOTOR1_PIN); //PORTID PIN ID
}

void motor1_off(void)
{
	CLEAR_BIT(MOTOR1_PORT, MOTOR1_PIN); //PORTID PINID
}


void motor_init(motor_config* config)
{
	DIO_setPin_Direction(config->PORT, config->PIN, OUTPUT);
	DIO_setPin_Value(config->PORT, config->PIN, LOW);

	//PWM Configs
	FPWM_TIMER2_INT_Init(F_CPU_CLOCK_256_FPWM2);
	FPWM_TIMER2_INT_setCallBack(&motor1_on, &motor1_off);

}

void motor_set_state(motor_config* config)
{
	if(config->motorstate == ON)
	{
		FPWM_TIMER2_INT_setDutyCycle(10);
	}
	else
	{
		FPWM_TIMER2_INT_setDutyCycle(0);
	}
}

void motor_increase_speed()
{

	motor_speed += 25;

	if(motor_speed > 100)
	{
		motor_speed = 100;

	}

	FPWM_TIMER2_INT_setDutyCycle(motor_speed);
}

void motor_decrease_speed()
{

	motor_speed -= 25;
	if(motor_speed < 0)
	{
		motor_speed = 0;
	}

	FPWM_TIMER2_INT_setDutyCycle(motor_speed);

}

void motor_increase_speed_by(u8_t percentage)
{
	if(percentage < 0 || percentage > 100)
		return;

	motor_speed += percentage;

	if(motor_speed > 100)
	{
		motor_speed = 100;

	}

	FPWM_TIMER2_INT_setDutyCycle(motor_speed);
}

void motor_decrease_speed_by(u8_t percentage)
{
	if(percentage < 0 || percentage > 100)
		return;

	motor_speed -= percentage;
	if(motor_speed < 0)
	{
		motor_speed = 0;
	}

	FPWM_TIMER2_INT_setDutyCycle(motor_speed);
}

void motor_set_speed(u8_t percentage)
{
	if(percentage < 0 || percentage > 100)
		return;

	FPWM_TIMER2_INT_setDutyCycle(percentage);
}
