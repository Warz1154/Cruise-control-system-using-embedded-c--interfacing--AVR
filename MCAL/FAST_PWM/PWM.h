/*
 * PWM.h
 *
 *  Created on: Dec 13, 2022
 *      Author: Ahmed
 */

#ifndef MCAL_FAST_PWM_PWM_H_
#define MCAL_FAST_PWM_PWM_H_

// Includes
#include "../../UTILS/types.h"
#include "../../UTILS/common_macros.h"

// Define
/*________________________________________________________________*/
/******************************************************************/
/*                        ERROR STATUS                            */
/******************************************************************/
#define FPWM_ERROR_t  u8_t
#define FPWM_E_OK     0
#define FPWM_E_NOK    1
/*______________________________________________________________________________________________________________________________*/


/*____________________Pre-scaler___________________*/
#define NO_CLOCK_FPWM2     						0
#define F_CPU_CLOCK_FPWM2   					1
#define F_CPU_CLOCK_8_FPWM2 					2
#define F_CPU_CLOCK_32_FPWM2					3
#define F_CPU_CLOCK_64_FPWM2					4
#define F_CPU_CLOCK_128_FPWM2					5
#define F_CPU_CLOCK_256_FPWM2		 			6
#define F_CPU_CLOCK_1024_FPWM2					7
/*_________________________________________________*/

/*____________________TIMER 2 OVERFLOW___________________*/
#define FPWM_TIMER2_OVERFLOW 256
/*_________________________________________________*/

/*_______________________________________________________________________________________________________________________________*/
/*Description: Fast PWM Initialization
 * Input     : pre-scaler (as defined above)
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
FPWM_ERROR_t FPWM_TIMER2_INT_Init(u8_t prescaler);

/*_______________________________________________________________________________________________________________________________*/
/*Description: Set the duty cycle of the device connected
 * Input     : duty cycle (0 - 100)
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
FPWM_ERROR_t FPWM_TIMER2_INT_setDutyCycle(u8_t dutyCycle);

/*_______________________________________________________________________________________________________________________________*/
/*Description: Set the callback function to be called in Timer2 OVF and CMP ISR functions
 * Input     : pointers to functions to be called
 * Output    : void
 * Use		 : To set callback for the device on function and device off function in interrupt handler
 *_______________________________________________________________________________________________________________________________*/
void FPWM_TIMER2_INT_setCallBack(void (*deviceOnFuncPtr) (void), void (*deviceOffFuncPtr) (void));

#endif /* MCAL_FAST_PWM_PWM_H_ */
