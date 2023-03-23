/*
 * interrupt.h
 *
 *  Created on: Nov 29, 2022
 *      Author: Ahmed
 */

#ifndef MCAL_INTERRUPT_INTERRUPT_H_
#define MCAL_INTERRUPT_INTERRUPT_H_

#include "../../UTILS/types.h"
#include "../../UTILS/common_macros.h"

// Null definition
#define NULL 0;

/***********************************************************************
 * This part of defines already defined in avr interrupt library
* 						so i have deleted it
 **********************************************************************/

// interrupt registers

/*#define SREG	( *( (volatile u8_t*) 0x5F ) )
#define GICR	( *( (volatile u8_t*) 0x5B ) )
#define MCUCR	( *( (volatile u8_t*) 0x55 ) )
#define MCUCSR	( *( (volatile u8_t*) 0x54 ) ) // EXT_INT2
*/

// MCUCR interrupt sense control bits definitions
// MCUCR is responsible for the triggering modes of EXT_INT0 and EXT_INT1
/*#define ISC00	0	// EXT_INT0
#define ISC01	1	// EXT_INT0
#define ISC10	2	// EXT_INT1
#define ISC11	3	// EXT_INT1

// MCUCSR interrupt sense control bit definition
// MCUCSR is responsible for the triggering modes of EXT_INT2
#define ISC2	6	// EXT_INT2

// GICR bits definitions
// GICR is responsible for enabling the individual interrupt of (EXT_INT0, EXT_INT1 and EXT_INT2)
#define INT2	5
#define INT0	6
#define INT1	7

// SREG bit definition
// SREG register will be used for enabling the global interrupt for the AVR
*/
#define GIE		7	// global interrupt enable bit (bit no. 7)
#define STATUS_REGISTER SREG


// EXT interrupt error handling enumeration
typedef enum EXTINT_Error
{
	EXTINT_E_NOK,
	EXTINT_E_OK
}EXTINT_Error_t;

// EXT interruptIs enumeration
typedef enum EXTINT_Channel
{
	EXT_INT0,
	EXT_INT1,
	EXT_INT2,
}EXTINT_Channel_t;

// EXT interrupts modes enumeration
typedef enum EXTINT_Mode
{
	LOW_LEVEL,			// for EXT_INT0 and EXT_INT1
	LOGICAL_CHANGE,		// for EXT_INT0 and EXT_INT1
	FALLING_EDGE,		// for EXT_INT0, EXT_INT1 and EXT_INT2
	RISING_EDGE,		// for EXT_INT0, EXT_INT1 and EXT_INT2
}EXTINT_Mode_t;


// functions prototypes
// enables the global interrupt bit in status register
void GLOBALInt_enable();

// disables the global interrupt bit in status register
void GLOBALInt_disbable();

// enables the individual interrupt of (EXT_INT0, EXT_INT1 or EXT_INT2)
EXTINT_Error_t EXTInt_enable(EXTINT_Channel_t Channel);

// disables the individual interrupt of (EXT_INT0, EXT_INT1 or EXT_INT2)
EXTINT_Error_t EXTInt_disable(EXTINT_Channel_t Channel);

/* sets the triggering mode for the external interrupt

Modes available:
	**For EXT_INT0 and EXT_INT1**
		1- The low level mode			(0v signal)
		2- Any logical change mode		(from 0v to VCC and vice verse, [like pressing a button then leaving it, it will interrupt while pressing then stops at the moment of leaving])
		3- The falling edge mode		(at the moment of giving 0v signal)
		4- The rising edge mode			(at the moment of giving VCC signal)
	**For EXT_INT2**
		1- The falling edge mode		(at the moment of giving 0v signal)
		2- The rising edge mode			(at the moment of giving VCC signal)
*/

EXTINT_Error_t EXTInt_Mode(EXTINT_Channel_t Channel, EXTINT_Mode_t Mode);

// used to set the ISR callback
EXTINT_Error_t EXTInt_setCallBack(EXTINT_Channel_t Channel, void (*funcPtr) (void));

// safety function to enable the global interrupt incase another peripheral disabled it
void checkGlobalInterrupt();

/*******************************EXTERNAL INTERRUPT NOTES**********************************************
 * Which triggering mode will you use with push button?
 * 	1- if rising edge:
 * 		---> SET the internal resistor as PULL DOWN
 * 		---> SET the external resistor as PULL UP
 *
 * 	2- if falling edge:
 * 		---> SET the internal resistor as PULL UP
 * 		---> SET the external resistor as PULL UP
 *
 * 	3- if LOGICAL_CHANGE:
 * 		---> SET the internal resistor as PULL UP
 * 		---> SET the external resistor as PULL UP
 *
 * 	4- if LOW_LEVEL:
 * 		---> SET the internal resistor as PULL UP
 * 		---> SET the external resistor as PULL UP
 ******************************************************************************************************/

#endif /* MCAL_INTERRUPT_INTERRUPT_H_ */
