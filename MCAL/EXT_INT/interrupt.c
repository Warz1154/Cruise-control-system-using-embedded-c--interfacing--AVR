// includes
#include "interrupt.h"
#include <avr/interrupt.h>

// Globals for setting callback functions
void (*EXTINT0_CallBackPtr) (void) = NULL;
void (*EXTINT1_CallBackPtr) (void) = NULL;
void (*EXTINT2_CallBackPtr) (void) = NULL;

void GLOBALInt_enable()
{
	SET_BIT(STATUS_REGISTER, GIE);
}

// disables the global interrupt bit in status register
void GLOBALInt_disbable()
{
	CLEAR_BIT(STATUS_REGISTER, GIE);
}

EXTINT_Error_t EXTInt_enable(EXTINT_Channel_t Channel)
{
	// Switching the external interrupt peripheral
	switch(Channel)
	{
		case EXT_INT0:
		// setting the int0 bit in the general interrupt control register to 1
		SET_BIT(GICR, INT0);
		break;
		case EXT_INT1:
		// setting the int1 bit in the general interrupt control register to 1
		SET_BIT(GICR, INT1);
		break;
		case EXT_INT2:
		// setting the int2 bit in the general interrupt control register to 1
		SET_BIT(GICR, INT2);
		break;
		default:
		// error handling
		return EXTINT_E_NOK;
	}
	return EXTINT_E_OK;
}

EXTINT_Error_t EXTInt_disable(EXTINT_Channel_t Channel)
{
	// Switching the external interrupt peripheral
	switch(Channel)
	{
		case EXT_INT0:
		// setting the int0 bit in the general interrupt control register to 0
		CLEAR_BIT(GICR, INT0);
		break;
		case EXT_INT1:
		// setting the int1 bit in the general interrupt control register to 0
		CLEAR_BIT(GICR, INT1);
		break;
		case EXT_INT2:
		// setting the int2 bit in the general interrupt control register to 0
		CLEAR_BIT(GICR, INT2);
		break;
		default:
		// error handling
		return EXTINT_E_NOK;
	}
	return EXTINT_E_OK;
}

EXTINT_Error_t EXTInt_Mode(EXTINT_Channel_t Channel, EXTINT_Mode_t Mode)
{
	// Switching the external interrupt peripheral
	switch(Channel)
	{
		case EXT_INT0:
		switch(Mode)
		{
			// Low level mode for EXT_INT0
			case LOW_LEVEL:
			CLEAR_BIT(MCUCR, ISC00);
			//setting ISC01 bit to zero in the MCU register
			CLEAR_BIT(MCUCR, ISC01);
			break;
			// Logical change mode for EXT_INT0
			case LOGICAL_CHANGE:
			SET_BIT(MCUCR, ISC00);
			CLEAR_BIT(MCUCR, ISC01);
			break;
			// Falling edge mode for EXT_INT0
			case FALLING_EDGE:
			CLEAR_BIT(MCUCR, ISC00);
			SET_BIT(MCUCR, ISC01);
			break;
			// Rising edge mode for EXT_INT0
			case RISING_EDGE:
			SET_BIT(MCUCR, ISC00);
			SET_BIT(MCUCR, ISC01);
			break;
			default:
			return EXTINT_E_NOK;
		}
		break;
		case EXT_INT1:
		switch(Mode)
		{
			// Low level mode for EXT_INT1
			case LOW_LEVEL:
			//setting ISC10 bit to zero in the MCU register
			CLEAR_BIT(MCUCR, ISC10);
			//setting ISC11 bit to zero in the MCU register
			CLEAR_BIT(MCUCR, ISC11);
			break;
			// Logical change mode for EXT_INT0
			case LOGICAL_CHANGE:
			SET_BIT(MCUCR, ISC10);
			CLEAR_BIT(MCUCR, ISC11);
			break;
			// Falling edge mode for EXT_INT0
			case FALLING_EDGE:
			CLEAR_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
			break;
			// Rising edge mode for EXT_INT0
			case RISING_EDGE:
			SET_BIT(MCUCR, ISC10);
			SET_BIT(MCUCR, ISC11);
			break;
			default:
			return EXTINT_E_NOK;
		}
		break;
		case EXT_INT2:
		switch(Mode)
		{
			// Falling edge mode for EXT_INT0
			case FALLING_EDGE:
			// setting the ISC2 bit in MCUCSR register to zero
			CLEAR_BIT(MCUCSR, ISC2);
			break;
			// Rising edge mode for EXT_INT0
			case RISING_EDGE:
			// setting the ISC2 bit in MCUCSR register to one
			SET_BIT(MCUCSR, ISC2);
			break;
			default:
			return EXTINT_E_NOK;
		}
		break;
		default:
		// error handling
		return EXTINT_E_NOK;
	}
	return EXTINT_E_OK;
}

EXTINT_Error_t EXTInt_setCallBack(EXTINT_Channel_t Channel, void (*funcPtr) (void))
{
	// return if null
	if(funcPtr == 0)
	{
		return EXTINT_E_NOK;
	}

	switch(Channel)
	{
		case EXT_INT0:
		EXTINT0_CallBackPtr = funcPtr;
		break;
		case EXT_INT1:
		EXTINT1_CallBackPtr = funcPtr;
		break;
		case EXT_INT2:
		EXTINT2_CallBackPtr = funcPtr;
		break;
		default:
		return EXTINT_E_NOK;
	}
	return EXTINT_E_OK;
}

// safety function
void checkGlobalInterrupt()
{
	if(!GET_BIT(STATUS_REGISTER, GIE))
		SET_BIT(STATUS_REGISTER, GIE);
}

// EXT_INT0 handler
// CPU will call the ISR automatic when the interrupt happens
ISR(__vector_1) // from avr interrupt.h
{
	if(EXTINT0_CallBackPtr != 0)
	{
		EXTINT0_CallBackPtr();
	}
}

// EXT_INT1 handler
ISR(__vector_2)
{
	if(EXTINT1_CallBackPtr != 0)
	{
		EXTINT1_CallBackPtr();
	}
}

// EXT_INT2 handler
ISR(__vector_3)
{
	if(EXTINT2_CallBackPtr != 0)
	{
		EXTINT2_CallBackPtr();
	}
}
