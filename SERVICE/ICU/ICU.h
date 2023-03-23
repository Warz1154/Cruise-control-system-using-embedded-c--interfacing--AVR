/*
 * ICU.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Ahmed
 */

#ifndef SERVICE_ICU_ICU_H_
#define SERVICE_ICU_ICU_H_

// Includes
#include "../../MCAL/TIMER/Timer_Interface.h"
#include "../../MCAL/EXT_INT/interrupt.h"

// Error Handling defines
#define ICU_E_NOK 	0
#define ICU_E_OK 	1
#define ICU_ERROR_t u8_t

// Timer configurations
#define TIMER_CHANNEL 			TIMER1
#define TIMER_INTERRUPT_MODE 	INTERRUPT
#define TIMER_WORK_MODE 		TIMER_MODE
#define TIMER_PRESCALER 		F_CPU_CLOCK_TIMER_1
#define TIMER_TICKCOUNTS		(TIMER1_OVERFLOW-1)

// Used to initalize the ICU with the timer and external interrupt peripherals
ICU_ERROR_t ICU_Init(EXTINT_Channel_t Channel);

// Used mainly to capture the time of the wave received on the external interrupt
// user doesnt have to use this function, its used by this peripheral only
void ICU_captureTime();

// User can use this function to read the value of the captured time.
void ICU_readCapturedTime(u16_t *capTime);

#endif /* SERVICE_ICU_ICU_H_ */
