/*
 * Timer_interface.c
 *
 *  Created on: Dec 3, 2022
 *      Author: Ahmed
 */


#include "../../UTILS/types.h"
#include "../../UTILS/MCU.h"
#include "../../UTILS/common_macros.h"
#include "Timer_interface.h"
#include <avr/interrupt.h>

u8_t TIMER0_Psc;
u8_t TIMER0_Interrupt_Mode;
void (*TIMER0_FuncPtr) (void) = 0;

u8_t TIMER1_Psc;
u8_t TIMER1_Interrupt_Mode;
void (*TIMER1_FuncPtr) (void) = 0;

u8_t TIMER2_Psc;
u8_t TIMER2_Interrupt_Mode;
void (*TIMER2_FuncPtr) (void) = 0;

extern void (*FPWM_Timer2OVFfuncPtr) (void);

extern u8_t Timer_Init(Str_TimerConfiguration_t* Confg_S)
{
	// we don't support counter mode
	if(Confg_S->Timer_Mode == COUNTER_MODE)
		return ERROR_NOK;

	switch(Confg_S->Timer_Channel)
	{
		case TIMER0:
		// internal buffer
		TIMER0_Psc = Confg_S->Timer_Psc;
		TIMER0_Interrupt_Mode = Confg_S->Interrupt_Mode;

		// enabling normal timer mode
		CLEAR_BIT(TCCR0, WGM00);
		CLEAR_BIT(TCCR0, WGM01);
		break;

		case TIMER1:
		// internal buffer
		TIMER1_Psc = Confg_S->Timer_Psc;
		TIMER1_Interrupt_Mode = Confg_S->Interrupt_Mode;

		// enabling normal timer mode
		// 16 bit timer --->  A:8-bit, B:8-bit
		// Timer control register 1 A
		CLEAR_BIT(TCCR1A, WGM10);
		CLEAR_BIT(TCCR1A, WGM11);

		// Timer Control Register 1 B
		CLEAR_BIT(TCCR1B, WGM12);
		CLEAR_BIT(TCCR1B, WGM13);
		break;

		case TIMER2:
		// internal buffer
		TIMER2_Psc = Confg_S->Timer_Psc;
		TIMER2_Interrupt_Mode = Confg_S->Interrupt_Mode;

		// enabling normal timer mode
		// Timer control register 2 (8-bit)
		CLEAR_BIT(TCCR2, WGM20);
		CLEAR_BIT(TCCR2, WGM21);
		break;

		default:
		return ERROR_NOK;
	}
	return ERROR_OK;
}

extern u8_t Timer_Start(u8_t Copy_u8_timerChannel, u16_t Copy_uint16_TickCounts)
{
	if(Copy_u8_timerChannel == TIMER0 || Copy_u8_timerChannel == TIMER2)
	{
		if(Copy_uint16_TickCounts > TIMER0_OVERFLOW || Copy_uint16_TickCounts < 0)
			return ERROR_NOK;
	}
	else if(Copy_u8_timerChannel == TIMER1)
	{
		if(Copy_uint16_TickCounts > TIMER1_OVERFLOW || Copy_uint16_TickCounts < 0)
			return ERROR_NOK;
	}
	else
	{
		return ERROR_NOK;
	}

	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
			// setting the ticks
			TCNT0 = TIMER0_OVERFLOW - Copy_uint16_TickCounts;
			// set the pre-scaler
			switch(TIMER0_Psc)
			{
				case NO_CLOCK_TIMER_0:
					CLEAR_BIT(TCCR0, CS00);
					CLEAR_BIT(TCCR0, CS01);
					CLEAR_BIT(TCCR0, CS02);
					break;

				case F_CPU_CLOCK_TIMER_0:
					SET_BIT(TCCR0, CS00);
					CLEAR_BIT(TCCR0, CS01);
					CLEAR_BIT(TCCR0, CS02);
					break;

				case F_CPU_CLOCK_8_TIMER_0:
					CLEAR_BIT(TCCR0, CS00);
					SET_BIT(TCCR0, CS01);
					CLEAR_BIT(TCCR0, CS02);
					break;

				case F_CPU_CLOCK_64_TIMER_0:
					SET_BIT(TCCR0, CS00);
					SET_BIT(TCCR0, CS01);
					CLEAR_BIT(TCCR0, CS02);
					break;

				case F_CPU_CLOCK_256_TIMER_0:
					CLEAR_BIT(TCCR0, CS00);
					CLEAR_BIT(TCCR0, CS01);
					SET_BIT(TCCR0, CS02);
					break;

				case F_CPU_CLOCK_1024_TIMER_0:
					SET_BIT(TCCR0, CS00);
					CLEAR_BIT(TCCR0, CS01);
					SET_BIT(TCCR0, CS02);
					break;

				case F_EXTERNAL_CLOCK_FALLING_TIMER_0:
					CLEAR_BIT(TCCR0, CS00);
					SET_BIT(TCCR0, CS01);
					SET_BIT(TCCR0, CS02);
					break;

				case F_EXTERNAL_CLOCK_RISING_TIMER_0:
					SET_BIT(TCCR0, CS00);
					SET_BIT(TCCR0, CS01);
					SET_BIT(TCCR0, CS02);
					break;

				default:
					return ERROR_NOK;
			}
			// enable INTERRUPTS incase of OVF_INT mode
			if(TIMER0_Interrupt_Mode == INTERRUPT)
			{
				// Enable global interrupt
				SET_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);

				// timer overflow interrupt enable
				SET_BIT(TIMSK, TOIE0);
			}
			else
			{
				u8_t flagStatus = 0;
				while(!flagStatus)
				{
					Timer_Get_FlagStatus(TIMER0, &flagStatus);
				}
				SET_BIT(TIFR, TOV0);
			}
			break;

		case TIMER1:
			// setting the ticks
			TCNT1 = TIMER1_OVERFLOW - Copy_uint16_TickCounts;
			// set the pre-scaler
			switch(TIMER1_Psc)
			{
				case NO_CLOCK_TIMER_1:
					CLEAR_BIT(TCCR1B, CS10);
					CLEAR_BIT(TCCR1B, CS11);
					CLEAR_BIT(TCCR1B, CS12);
					break;

				case F_CPU_CLOCK_TIMER_1:
					SET_BIT(TCCR1B, CS10);
					CLEAR_BIT(TCCR1B, CS11);
					CLEAR_BIT(TCCR1B, CS12);
					break;

				case F_CPU_CLOCK_8_TIMER_1:
					CLEAR_BIT(TCCR1B, CS10);
					SET_BIT(TCCR1B, CS11);
					CLEAR_BIT(TCCR1B, CS12);
					break;

				case F_CPU_CLOCK_64_TIMER_1:
					SET_BIT(TCCR1B, CS10);
					SET_BIT(TCCR1B, CS11);
					CLEAR_BIT(TCCR1B, CS12);
					break;

				case F_CPU_CLOCK_256_TIMER_1:
					CLEAR_BIT(TCCR1B, CS10);
					CLEAR_BIT(TCCR1B, CS11);
					SET_BIT(TCCR1B, CS12);
					break;

				case F_CPU_CLOCK_1024_TIMER_1:
					SET_BIT(TCCR1B, CS10);
					CLEAR_BIT(TCCR1B, CS11);
					SET_BIT(TCCR1B, CS12);
					break;

				case F_EXTERNAL_CLOCK_FALLING_TIMER_1:
					CLEAR_BIT(TCCR1B, CS10);
					SET_BIT(TCCR1B, CS11);
					SET_BIT(TCCR1B, CS12);
					break;

				case F_EXTERNAL_CLOCK_RISING_TIMER_1:
					SET_BIT(TCCR1B, CS10);
					SET_BIT(TCCR1B, CS11);
					SET_BIT(TCCR1B, CS12);
					break;

				default:
					return ERROR_NOK;
			}
			// enable INTERRUPTS incase of OVF_INT mode
			if(TIMER1_Interrupt_Mode == INTERRUPT)
			{
				// Enable global interrupt
				SET_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);

				// timer overflow interrupt enable
				SET_BIT(TIMSK, TOIE1);
			}
			else
			{
				u8_t flagStatus = 0;
				while(!flagStatus)
				{
					Timer_Get_FlagStatus(TIMER1, &flagStatus);
				}
				SET_BIT(TIFR, TOV1);
			}
			break;

		case TIMER2:
			// setting the ticks
			TCNT2 = TIMER2_OVERFLOW - Copy_uint16_TickCounts;

			// set the pre-scaler
			switch(TIMER2_Psc)
			{
				case NO_CLOCK_TIMER_2:
					CLEAR_BIT(TCCR2, CS20);
					CLEAR_BIT(TCCR2, CS21);
					CLEAR_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_TIMER_2:
					SET_BIT(TCCR2, CS20);
					CLEAR_BIT(TCCR2, CS21);
					CLEAR_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_8_TIMER_2:
					CLEAR_BIT(TCCR2, CS20);
					SET_BIT(TCCR2, CS21);
					CLEAR_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_32_TIMER_2:
					SET_BIT(TCCR2, CS20);
					SET_BIT(TCCR2, CS21);
					CLEAR_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_64_TIMER_2:
					CLEAR_BIT(TCCR2, CS20);
					CLEAR_BIT(TCCR2, CS21);
					SET_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_128_TIMER_2:
					SET_BIT(TCCR2, CS20);
					CLEAR_BIT(TCCR2, CS21);
					SET_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_256_TIMER_2:
					CLEAR_BIT(TCCR2, CS20);
					SET_BIT(TCCR2, CS21);
					SET_BIT(TCCR2, CS22);
					break;

				case F_CPU_CLOCK_1024_TIMER_2:
					SET_BIT(TCCR2, CS20);
					SET_BIT(TCCR2, CS21);
					SET_BIT(TCCR2, CS22);
					break;

				default:
					return ERROR_NOK;
			}
			// enable INTERRUPTS incase of OVF_INT mode
			if(TIMER2_Interrupt_Mode == INTERRUPT)
			{
				// Enable global interrupt
				SET_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);

				// timer2 overflow interrupt enable
				SET_BIT(TIMSK, TOIE2);
			}
			else
			{
				u8_t flagStatus = 0;
				while(!flagStatus)
				{
					Timer_Get_FlagStatus(TIMER2, &flagStatus);
				}
				SET_BIT(TIFR, TOV2);
			}
			break;
	}

	return ERROR_OK;
}


extern u8_t Timer_Stop(u8_t Copy_u8_timerChannel)
{
	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
		if(TIMER0_Interrupt_Mode == INTERRUPT)
		{
			// disable global interrupt
			CLEAR_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);  // WHY SHOULD I DO THAT? INTERRUPT PERIPHERAL MAYBE USED...
			// disable ovf interrupt
			CLEAR_BIT(TIMSK, TOIE0);
		}
		/***************QUESTION*************************************
		*
		* what if it was polling mode???
		* The else of the if.....
		*		---> nothing to do
		************************************************************/
		break;

		case TIMER1:
		if(TIMER1_Interrupt_Mode == INTERRUPT)
		{
			// disable global interrupt
			CLEAR_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);  // WHY SHOULD I DO THAT? INTERRUPT PERIPHERAL MAYBE USED...
			// disable ovf interrupt
			CLEAR_BIT(TIMSK, TOIE1);
		}
		break;

		case TIMER2:
		if(TIMER2_Interrupt_Mode == INTERRUPT)
		{
			// disable global interrupt
			CLEAR_BIT(STATUS_REGISTER, GLOBAL_INTERRUPT_BIT);  // WHY SHOULD I DO THAT? INTERRUPT PERIPHERAL MAYBE USED...
			// disable ovf interrupt
			CLEAR_BIT(TIMSK, TOIE2);
		}
		break;

		default:
		return ERROR_NOK;
	}
	return ERROR_OK;
}


// should i reset timer counter register only??
// or should i set logical one to ovf flag register TIFR to clear it?

extern u8_t Timer_Reset(u8_t Copy_u8_timerChannel, u16_t Copy_uint16_TickCounts)
{
	if(Copy_u8_timerChannel == TIMER0 || Copy_u8_timerChannel == TIMER2)
	{
		if(Copy_uint16_TickCounts > TIMER0_OVERFLOW || Copy_uint16_TickCounts < 0)
			return ERROR_NOK;
	}
	else
	{
		if(Copy_uint16_TickCounts > TIMER1_OVERFLOW || Copy_uint16_TickCounts < 0)
			return ERROR_NOK;
	}

	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
		TCNT0 = TIMER0_OVERFLOW - Copy_uint16_TickCounts;
		break;

		case TIMER1:
		TCNT1 = TIMER1_OVERFLOW - Copy_uint16_TickCounts;;
		break;

		case TIMER2:
		TCNT2 = TIMER2_OVERFLOW - Copy_uint16_TickCounts;;
		break;

		default:
		return ERROR_NOK;
	}
	return ERROR_OK;
}

extern u8_t Timer_Get_TickTime(u8_t Copy_u8_timerChannel, u16_t *Copy_uint8Ptr_TimerTickTime)
{
	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
		*Copy_uint8Ptr_TimerTickTime = TCNT0;
		break;

		case TIMER1:
		*Copy_uint8Ptr_TimerTickTime = TCNT1;
		break;

		case TIMER2:
		*Copy_uint8Ptr_TimerTickTime = TCNT2;
		break;

		default:
		return ERROR_NOK;
	}
	return ERROR_OK;
}

extern u8_t Timer_Get_FlagStatus(u8_t Copy_u8_timerChannel, u8_t *Copy_uint8Ptr_FlagStatus)
{
	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
		// getting the Timer overflow bit value from the Timer interrupt flag register
		*Copy_uint8Ptr_FlagStatus = GET_BIT(TIFR, TOV0);
		break;

		case TIMER1:
		// getting the Timer overflow bit value from the Timer interrupt flag register
		*Copy_uint8Ptr_FlagStatus = GET_BIT(TIFR, TOV1);
		break;

		case TIMER2:
		// getting the Timer overflow bit value from the Timer interrupt flag register
		*Copy_uint8Ptr_FlagStatus = GET_BIT(TIFR, TOV2);
		break;

		default:
		return ERROR_NOK;
	}
	return ERROR_OK;
}

u8_t TIMER_SetCallBack(u8_t Copy_u8_timerChannel,void (*Copy_FunctionAddress)(void))
{
	switch(Copy_u8_timerChannel)
	{
		case TIMER0:
			TIMER0_FuncPtr = Copy_FunctionAddress;
			break;

		case TIMER1:
			TIMER1_FuncPtr = Copy_FunctionAddress;
			break;

		case TIMER2:
			TIMER2_FuncPtr = Copy_FunctionAddress;
			break;

		default:
			return ERROR_NOK;
	}
	return ERROR_OK;
}

// Timer0 overflow interrupt handler
ISR(__vector_11)
{
	if(TIMER0_FuncPtr != 0)
		TIMER0_FuncPtr();
}

// Timer1 overflow interrupt handler
ISR(__vector_9)
{
	if(TIMER1_FuncPtr != 0)
		TIMER1_FuncPtr();
}

// Timer2 overflow interrupt handler
ISR(__vector_5)
{
	if(TIMER2_FuncPtr != 0)
		TIMER2_FuncPtr();

	if(FPWM_Timer2OVFfuncPtr != 0)
		FPWM_Timer2OVFfuncPtr();
}
