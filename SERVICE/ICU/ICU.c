/*
 * ICU.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Ahmed
 */

#include "ICU.h"
#include "../../HAL/LCD/LCD.H"

u8_t interrupt_counter = 0;
u8_t EXTInt_Chan;
u8_t timerStarted = 0;
u16_t capturedTime = 0;

ICU_ERROR_t ICU_Init(EXTINT_Channel_t Channel)
{
	// Errors checking (guards)
	if(Channel != EXT_INT0 && Channel != EXT_INT1 && Channel != EXT_INT2)
		return ICU_E_NOK;

	// timer1 configuration structure
	Str_TimerConfiguration_t T_confg;
	T_confg.Interrupt_Mode = TIMER_INTERRUPT_MODE;
	T_confg.Timer_Channel = TIMER_CHANNEL;
	T_confg.Timer_Mode = TIMER_WORK_MODE;
	T_confg.Timer_Psc = TIMER_PRESCALER;

	// init timer1
	Timer_Init(&T_confg);

	// enable global interrupt
	GLOBALInt_enable();

	// enable external interrupt 1
	EXTInt_enable(Channel);

	// configure the ext int as rising edge
	EXTInt_Mode(Channel, RISING_EDGE);

	// calling Time_on_read function when an interrupt occurs (ultrasonic echo)
	EXTInt_setCallBack(Channel, &ICU_captureTime);

	// Ecternal interrupt channel internal buffer
	EXTInt_Chan = Channel;
	return ICU_E_OK;
}

void ICU_captureTime()
{
	// if first interrupt
	if(interrupt_counter == 0)
	{
		// start timer
		Timer_Start(TIMER_CHANNEL, TIMER_TICKCOUNTS);
		//configure as falling
		EXTInt_Mode(EXTInt_Chan, FALLING_EDGE);
		// count interrupts
		interrupt_counter++;
	}
	// if second interrupt
	else
	{
		// get counted ticks
		Timer_Get_TickTime(TIMER_CHANNEL, &capturedTime);
		// reconfigure the interrupt as rising again
		EXTInt_Mode(EXTInt_Chan, RISING_EDGE);
		// reset interrupt counter
		interrupt_counter = 0;
	}
}

void ICU_readCapturedTime(u16_t *capTime)
{
	*capTime = capturedTime;
}
