#include "DIO.h"


//PORT FUNCTION IMPLEMENATION

//set Port Direction
Error_t DIO_setPort_Direction(DIO_PORT_ID port_ID, DIO_DIRECTION direction)
{
	if(direction == OUTPUT)
	{
		switch(port_ID)
		{
			case DPORTA: DDRA = 0xFF; break;
			case DPORTB: DDRB = 0xFF; break;
			case DPORTC: DDRC = 0xFF; break;
			case DPORTD: DDRD = 0xFF; break;
			default: return E_N_OK;
		}
	}
	else if (direction == INPUT)
	{
		switch(port_ID)
		{
			case DPORTA: DDRA = 0x00; break;
			case DPORTB: DDRB = 0x00; break;
			case DPORTC: DDRC = 0x00; break;
			case DPORTD: DDRD = 0x00; break;
			default: return E_N_OK;
		}
	}
	else
	{
		return E_N_OK;
	}
	return E_OK;
}


//SET PORT VALUE
Error_t DIO_setPort_Value(DIO_PORT_ID port_ID, u8_t value)
{
	switch(port_ID)
	{
		case DPORTA: PORTA = value; break;
		case DPORTB: PORTB = value; break;
		case DPORTC: PORTC = value; break;
		case DPORTD: PORTD = value; break;
		default: return E_N_OK;
	}
	return E_OK;
}


Error_t GetPort_Value(DIO_PORT_ID port_ID, u8_t *PtrPortValue)
{
	switch(port_ID)
	{
		case DPORTA: *PtrPortValue = PINA; break;
		case DPORTB: *PtrPortValue = PINB; break;
		case DPORTC: *PtrPortValue = PINC; break;
		case DPORTD: *PtrPortValue = PIND; break;
		default: return E_N_OK;
	}
	return E_OK;
}


//PIN FUNCTIONS

//-------------------------------------------------------------------------------------------------------------------------------

//SET PIN DIRECTION
Error_t DIO_setPin_Direction(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, DIO_DIRECTION direction)
{
	if(pin_ID > PIN7 || pin_ID < PIN0 ) //CHECKS IF THE PIN is WITHIN THE VALID RANGE
			return E_N_OK;

	if(direction == OUTPUT)
	{
		switch(port_ID)
		{
			case DPORTA: SET_BIT(DDRA, pin_ID); break;
			case DPORTB: SET_BIT(DDRB, pin_ID); break;
			case DPORTC: SET_BIT(DDRC, pin_ID); break;
			case DPORTD: SET_BIT(DDRD, pin_ID); break;
			default: return E_N_OK;
		}
	}
	else if(direction == INPUT)
	{
		switch(port_ID)
		{
			case DPORTA: CLEAR_BIT(DDRA, pin_ID); break;
			case DPORTB: CLEAR_BIT(DDRB, pin_ID); break;
			case DPORTC: CLEAR_BIT(DDRC, pin_ID); break;
			case DPORTD: CLEAR_BIT(DDRD, pin_ID); break;
			default: return E_N_OK;
		}
	}
	else
	{
		return E_N_OK;
	}
	return E_OK;
}

//set Pin Value

Error_t DIO_setPin_Value(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, DIO_VALUE value)
{
	if(pin_ID > PIN7 || pin_ID < PIN0 ) //CHECKS IF THE PIN is WITHIN THE VALID RANGE
			return E_N_OK;

	if(value == HIGH)
	{
		switch(port_ID)
		{
			case DPORTA: SET_BIT(PORTA, pin_ID); break;
			case DPORTB: SET_BIT(PORTB, pin_ID); break;
			case DPORTC: SET_BIT(PORTC, pin_ID); break;
			case DPORTD: SET_BIT(PORTD, pin_ID); break;
			default: return E_N_OK;
		}
	}
	else if(value == LOW)
	{
		switch(port_ID)
		{
			case DPORTA: CLEAR_BIT(PORTA, pin_ID); break;
			case DPORTB: CLEAR_BIT(PORTB, pin_ID); break;
			case DPORTC: CLEAR_BIT(PORTC, pin_ID); break;
			case DPORTD: CLEAR_BIT(PORTD, pin_ID); break;
			default: return E_N_OK;
			}
	}
	else
	{
		return E_N_OK;
	}
	return E_OK;
}

//GET PIN VALUE

Error_t DIO_getPin_Value(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, u8_t *PtrPinValue)
{
	if(pin_ID > PIN7 || pin_ID < PIN0 ) //CHECKS IF THE PIN is WITHIN THE VALID RANGE
			return E_N_OK;

	switch(port_ID)
	{
		case DPORTA: *PtrPinValue = GET_BIT(PINA, pin_ID); break;
		case DPORTB: *PtrPinValue = GET_BIT(PINB, pin_ID); break;
		case DPORTC: *PtrPinValue = GET_BIT(PINC, pin_ID); break;
		case DPORTD: *PtrPinValue = GET_BIT(PIND, pin_ID); break;
		default: return E_N_OK;
	}

	return E_OK;
}


