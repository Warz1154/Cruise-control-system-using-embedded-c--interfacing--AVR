
/*
 * PWM.c
 *
 *  Created on: Dec 13, 2022
 *      Author: Ahmed
 */

#include "PWM.h"
#include <avr/interrupt.h>

void (*Timer2CMPfuncPtr) (void) = 0;
void (*FPWM_Timer2OVFfuncPtr) (void) = 0;

FPWM_ERROR_t FPWM_TIMER2_INT_Init(u8_t prescaler)
{
	// timer2 as a fast PWM
	SET_BIT(TCCR2, WGM20);
	SET_BIT(TCCR2, WGM21);

	// timer2 fast pwm select inverting mode
	CLEAR_BIT(TCCR2, COM20);
	SET_BIT(TCCR2, COM21);

	// timer ovf and ctc interrupt enable
	SET_BIT(TIMSK, TOIE2); // TIMER2 ovf int
	SET_BIT(TIMSK, OCIE2); // TIMER2 ctc int

	// Global interrupt enable
	SET_BIT(SREG, 7);

	// PRESCALER
	switch(prescaler)
	{
		case NO_CLOCK_FPWM2:
			CLEAR_BIT(TCCR2, CS20);
			CLEAR_BIT(TCCR2, CS21);
			CLEAR_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_FPWM2:
			SET_BIT(TCCR2, CS20);
			CLEAR_BIT(TCCR2, CS21);
			CLEAR_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_8_FPWM2:
			CLEAR_BIT(TCCR2, CS20);
			SET_BIT(TCCR2, CS21);
			CLEAR_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_32_FPWM2:
			SET_BIT(TCCR2, CS20);
			SET_BIT(TCCR2, CS21);
			CLEAR_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_64_FPWM2:
			CLEAR_BIT(TCCR2, CS20);
			CLEAR_BIT(TCCR2, CS21);
			SET_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_128_FPWM2:
			SET_BIT(TCCR2, CS20);
			CLEAR_BIT(TCCR2, CS21);
			SET_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_256_FPWM2:
			CLEAR_BIT(TCCR2, CS20);
			SET_BIT(TCCR2, CS21);
			SET_BIT(TCCR2, CS22);
			break;

		case F_CPU_CLOCK_1024_FPWM2:
			SET_BIT(TCCR2, CS20);
			SET_BIT(TCCR2, CS21);
			SET_BIT(TCCR2, CS22);
			break;

		default:
			return FPWM_E_NOK;
	}

	return FPWM_E_OK;
}

FPWM_ERROR_t FPWM_TIMER2_INT_setDutyCycle(u8_t dutyCycle)
{
	if(dutyCycle < 0 || dutyCycle > 100)
		return FPWM_E_NOK;

	u16_t value = (FPWM_TIMER2_OVERFLOW - (FPWM_TIMER2_OVERFLOW * ((float)dutyCycle/100)));

	if(value == 0)
		OCR2 = 1; // 100% Duty cycle
	else
		OCR2 = value; // a specific duty cycle rather than 100%

	return FPWM_E_OK;
}

void FPWM_TIMER2_INT_setCallBack(void (*deviceOnFuncPtr) (void), void (*deviceOffFuncPtr) (void))
{
	Timer2CMPfuncPtr = deviceOnFuncPtr;
	FPWM_Timer2OVFfuncPtr = deviceOffFuncPtr;
}

// Timer2 CTC Interrupt
ISR(__vector_4)
{
	if(Timer2CMPfuncPtr != 0)
		Timer2CMPfuncPtr();
}

// Timer2 OVF Interrupt
// Moved to timer driver
/*ISR(__vector_5)
{
	if(Timer2OVFfuncPtr != 0)
		Timer2OVFfuncPtr();
}
*/
