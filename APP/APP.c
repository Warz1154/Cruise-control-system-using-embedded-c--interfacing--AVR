/*
 * APP.c
 *
 *  Created on: Dec 16, 2022
 *      Author: Ahmed
 */


#include "APP.h"

str_BTN_config_t BTN_INC_confg_s, BTN_DEC_confg_s, BTN_BRK_confg_s;
motor_config M_confg_s;
str_US_config_t US_confg_s;

Str_TimerConfiguration_t Timer_confg_s;

// global signals
u8_t Car_Ignition_On = 0;
u8_t APP_Mode;
u32_t Timer_Total_Overflows = 0;
u64_t Timer_Total_Ticks_ms = 0;

u8_t US_Push_But_Feat_Flag = 0;
u64_t US_lastPushTickTime = 0;

void APP_init()
{
	// Increment button config
	BTN_INC_confg_s.Port = BUTTON_INC_PORT;
	BTN_INC_confg_s.Pin = BUTTON_INC_PIN;

	// Decrement button config
	BTN_DEC_confg_s.Port = BUTTON_DEC_PORT;
	BTN_DEC_confg_s.Pin = BUTTON_DEC_PIN;

	// Brake button config
	/*BTN_BRK_confg_s.Port = BUTTON_BRAKE_PORT;
	BTN_BRK_confg_s.Pin = BUTTON_BRAKE_PIN;
*/

	// buttons initialization
	/*BUTTON_Init(&BTN_INC_confg_s);
	BUTTON_Init(&BTN_DEC_confg_s);*/
	//BUTTON_Init(&BTN_BRK_confg_s);

	GLOBALInt_enable();

	// Button INCREMENT init
	EXTInt_enable(EXT_INT0);
	EXTInt_Mode(EXT_INT0, FALLING_EDGE);
	SET_BIT(PORTD, 2); // pull up
	EXTInt_setCallBack(EXT_INT0, &APP_BTN_INC_readState);

	// button decrement init
	EXTInt_enable(EXT_INT2);
	EXTInt_Mode(EXT_INT2, FALLING_EDGE);
	SET_BIT(PORTB, 2); // pull up
	EXTInt_setCallBack(EXT_INT2, &APP_BTN_DEC_readState);

	// initializing ultrasonic sensor
	US_confg_s.Echo_Channel = US_CHANNEL_1;
	US_confg_s.Trigger_Port = US_TRIGGER_PORT;
	US_confg_s.Trigger_Pin = US_TRIGGER_PIN;

	// Activating the trigger pin as ouput
	DIO_setPin_Direction(US_TRIGGER_PORT, US_TRIGGER_PIN, OUTPUT);
	// ultrasonic init
	ULTRASONIC_Init(&US_confg_s);

	// MOTOR CONFIG
	M_confg_s.PORT = MOTOR_PORT;
	M_confg_s.PIN = MOTOR_PIN;
	M_confg_s.motorstate = ON;

	// MOTOR init
	motor_init(&M_confg_s);

	// Start engine
	motor_set_state(&M_confg_s);

	// motor started
	Car_Ignition_On = 1;

	// Switch to default mode at initialization of the system
	APP_Mode = APP_DEFAULT_MODE;

	// TIMER CONFIG
	Timer_confg_s.Interrupt_Mode = APP_TIMER_INTERRUPT_MODE;
	Timer_confg_s.Timer_Channel = APP_TIMER_CHANNEL;
	Timer_confg_s.Timer_Mode = APP_TIMER_MODE;
	Timer_confg_s.Timer_Psc = APP_TIMER_PSC;

	// TIMER INIT
	Timer_Init(&Timer_confg_s);
	// TIMER CALL BACK FUNCTION INCASE OF INTERRUPT
	TIMER_SetCallBack(APP_TIMER_CHANNEL, &APP_ticksIncrement);
	// STARTING TIMER
	Timer_Start(APP_TIMER_CHANNEL, APP_TIMER_TICKS);
}

void APP_readSensors()
{
	// read break button state
	/*BUTTON_readState(&BTN_BRK_confg_s);
	_delay_ms(50);

	if(BTN_BRK_confg_s.State == PRESSED)
	{
		if(BTN_INC_confg_s.State == NOT_PRESSED && BTN_DEC_confg_s.State == NOT_PRESSED)
		{
			motor_decrease_speed();
			APP_Mode = APP_OFF_MODE;
			US_Push_But_Feat_Flag = 0;
		}
		return;
	}*/

	switch(APP_Mode)
	{
		case APP_DEFAULT_MODE:
			if(BTN_INC_confg_s.State == PRESSED)
			{
				if(BTN_DEC_confg_s.State == NOT_PRESSED)
				{
					motor_increase_speed();
					US_Push_But_Feat_Flag = 0;
				}
				else
				{
					return;
				}
			}
			else if(BTN_DEC_confg_s.State == PRESSED)
			{
				if(BTN_INC_confg_s.State == NOT_PRESSED)
				{
					motor_decrease_speed();
					US_Push_But_Feat_Flag = 0;
				}
				else
				{
					return;
				}
			}
			else
			{
				if(!US_Push_But_Feat_Flag)
				{
					US_Push_But_Feat_Flag = 1;
					APP_getCurrentTicks_ms(&US_lastPushTickTime);
				}
				else
				{
					u64_t currentTicks;
					APP_getCurrentTicks_ms(&currentTicks);
					if(currentTicks - US_lastPushTickTime >= 5000)
					{
						APP_Mode = APP_ULTRASONIC_MODE;
						US_Push_But_Feat_Flag = 0;
					}
				}
			}
			BTN_INC_confg_s.State = 0;
			BTN_DEC_confg_s.State = 0;
			break;
		case APP_ULTRASONIC_MODE:
			if(BTN_INC_confg_s.State == PRESSED)
			{
				if(BTN_DEC_confg_s.State == NOT_PRESSED)
				{
					motor_increase_speed();
					US_Push_But_Feat_Flag = 0;
					APP_Mode = APP_DEFAULT_MODE;
				}
				else
				{
					return;
				}
			}
			else if(BTN_DEC_confg_s.State == PRESSED)
			{
				if(BTN_INC_confg_s.State == NOT_PRESSED)
				{
					motor_decrease_speed();
					US_Push_But_Feat_Flag = 0;
					APP_Mode = APP_DEFAULT_MODE;
				}
				else
				{
					return;
				}
			}
			// ultrasonic requirements
			else
			{
				// read ultrasonic distance
				ULTRASONIC_readDistance(&US_confg_s);
				if(US_confg_s.distance >= 80)
				{
					motor_set_speed(100);
				}
				else if (US_confg_s.distance >= 50 && US_confg_s.distance < 80)
				{
					motor_set_speed(75);
				}
				else if (US_confg_s.distance >= 20 && US_confg_s.distance < 50)
				{
					motor_set_speed(50);
				}
				else if (US_confg_s.distance < 20)
				{
					motor_decrease_speed_by(5);
				}
			}
			BTN_INC_confg_s.State = 0;
			BTN_DEC_confg_s.State = 0;
			break;
		case APP_OFF_MODE:
			break;
	}
}

void APP_ticksIncrement(void)
{
	Timer_Total_Overflows++;
	Timer_Total_Ticks_ms = Timer_Total_Overflows * APP_TIMER_OVF_MS;
}

void APP_getCurrentTicks_ms(u64_t *timer_ms)
{
	*timer_ms =  Timer_Total_Ticks_ms;
}

void APP_BTN_INC_readState()
{
	BTN_INC_confg_s.State = 1;
}

void APP_BTN_DEC_readState()
{
	BTN_DEC_confg_s.State = 1;
}
