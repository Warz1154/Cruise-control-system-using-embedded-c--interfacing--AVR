/*
 * UART.c
 *
 *  Created on: Dec 11, 2022
 *      Author: Ahmed
 */

#include "../../UTILS/types.h"
#include "../../UTILS/MCU.h"
#include "../../UTILS/common_macros.h"
#include "UART.h"
#include <avr/interrupt.h>

// Globals
u8_t UART_FlagAction; 				// For internal buffer of uart mode
u8_t UART_Data_Transmit;			// buffer to store data to be sent
u8_t *UART_Data_Received_ptr = 0;	// pointer to store the address of the var the data should be received in, incase of interrupt mode
u8_t UART_DataSent_Status = UART_DATA_SENT;				// Global flag to ensure that the data sent for avoiding data sending repetition

void (*UART_TX_Funcptr) (void) = 0;	// TX pointer to function to be called in TX ISR
void (*UART_RX_Funcptr) (void) = 0;	// RX pointer to function to be called in RX ISR

extern UART_Error_t UART_init(str_UartConfg_t * uart_config)
{
	// function guards for error handling
	if(uart_config->u8_FlagAction != UART_POLLING && uart_config->u8_FlagAction != UART_INTERRUPT )
		return UART_E_NOK;

	// Baud rate valid range checking
	if(uart_config->u32_BaudRate < 0 || uart_config->u32_BaudRate > 256000)
		return UART_E_NOK;

	// Internal buffer for later use
	UART_FlagAction = uart_config->u8_FlagAction;

	// UART Receiver enable
	SET_BIT(UCSRB, RXEN);
	// UART Transmitter enable
	SET_BIT(UCSRB, TXEN);

	// UART Asynchronous Mode select
	CLEAR_BIT(UCSRC, UMSEL);

	// Using UCSRC
	SET_BIT(UCSRC, URSEL);

	// UART Character size selection
	switch(uart_config->u8_DataBits)
	{
		case UART_5_BIT_MODE:
		CLEAR_BIT(UCSRC, UCSZ0);
		CLEAR_BIT(UCSRC, UCSZ1);
		CLEAR_BIT(UCSRB, UCSZ2);
		break;

		case UART_6_BIT_MODE:
		SET_BIT(UCSRC, UCSZ0);
		CLEAR_BIT(UCSRC, UCSZ1);
		CLEAR_BIT(UCSRB, UCSZ2);
		break;

		case UART_7_BIT_MODE:
		CLEAR_BIT(UCSRC, UCSZ0);
		SET_BIT(UCSRC, UCSZ1);
		CLEAR_BIT(UCSRB, UCSZ2);
		break;

		case UART_8_BIT_MODE:
		SET_BIT(UCSRC, UCSZ0);
		SET_BIT(UCSRC, UCSZ1);
		CLEAR_BIT(UCSRB, UCSZ2);
		break;

		case UART_9_BIT_MODE:
		SET_BIT(UCSRC, UCSZ0);
		SET_BIT(UCSRC, UCSZ1);
		SET_BIT(UCSRB, UCSZ2);
		break;

		default:
		return UART_E_NOK;
	}

	// Interrupt configurations
	if(uart_config->u8_FlagAction == UART_INTERRUPT)
	{
		// Enable Global interrupt
		SET_BIT(SREG, 7);
		// RX complete interrupt enable
		SET_BIT(UCSRB, RXCIE);
		// TX complete interrupt enable
		SET_BIT(UCSRB, TXCIE);
	}

	// Temp var to store the equation result
	u16_t UBRR_val = ((F_CPU)/((uart_config->u32_BaudRate)*16UL)-1);

	// Splitting the 12 bits result of the above equation into the UBRRL & UBRRH registers
	UBRRH = (UBRR_val>>8); 	// shift the register right by 8 bits
	UBRRL = UBRR_val;		// set baud rate

	return UART_E_OK;
}

extern UART_Error_t UART_sendByte(const u8_t u8_Data)
{
	if(u8_Data < 0 || u8_Data > 255)
		return UART_E_NOK;

	if(UART_FlagAction == UART_POLLING)
	{
		// WHILE the UDR is not empty stuck until it is empty
		while(!GET_BIT(UCSRA, UDRE));
		// then put the data in the empty UDR
		UDR = u8_Data;
	}
	else if(UART_FlagAction == UART_INTERRUPT)
	{
		// storing the data to be sent in a global buffer
		UART_Data_Transmit = u8_Data;
		// setting data sent flag to zero for later ISR later use
		UART_DataSent_Status = UART_DATA_NOT_SENT;
		// Enable the uart data register empty interrupt enable
		// so we can send the data when it gets fired as we are in interrupt mode
		SET_BIT(UCSRB, UDRIE);
	}
	else
	{
		return UART_E_NOK;
	}
	return UART_E_OK;
}

extern UART_Error_t UART_recieveByte(u8_t *u8_Data)
{
	if(UART_FlagAction == UART_POLLING)
	{
		while(!GET_BIT(UCSRA, RXC));	// Wait until data is received
		*u8_Data = UDR;					// return the data into the pointer
	}
	else if(UART_FlagAction == UART_INTERRUPT)
	{
		// internal bufffer to store the address of the data var so we can return UDR to it in ISR
		UART_Data_Received_ptr = u8_Data;
	}
	else
	{
		return UART_E_NOK;
	}
	return UART_E_OK;
}


// when transmit completed, call a specific function
extern UART_Error_t UART_TX_SetCallBack(void (*Copy_FunctionAddress)(void))
{
	// return if null
	if(Copy_FunctionAddress == 0)
		return UART_E_NOK;

	// pointer to be called in ISR
	UART_TX_Funcptr = Copy_FunctionAddress;
	return UART_E_OK;
}

extern UART_Error_t UART_RX_SetCallBack(void (*Copy_FunctionAddress)(void))
{
	// return if null
	if(Copy_FunctionAddress == 0)
		return UART_E_NOK;

	UART_RX_Funcptr = Copy_FunctionAddress;
	return UART_E_OK;
}

// UDRE interrupt handler
// for sending data to UDR register when it is empty and ready
ISR(__vector_14)
{
	// Sending the data from the global var to the UDR register so that it can be transmitted
	if(UART_DataSent_Status == UART_DATA_NOT_SENT)
	{
		// write to the UDR register the data to be sent
		UDR = UART_Data_Transmit;
		// fire the software data sent flag so ISR will not be called again.
		UART_DataSent_Status = UART_DATA_SENT;
		// Clear the UDR empty flag to avoid RX frame error message.
		CLEAR_BIT(UCSRB, UDRIE);
	}
}

// RXC interrupt handler
// for receiving data from UDR register when it receives it
ISR(__vector_13)
{
	// Storing the data received in the UDR in the Data receiver global pointer
	*UART_Data_Received_ptr = UDR;

	// Call a specific function when data is received successfully
	if(UART_RX_Funcptr != 0)	// call the function if not null
		UART_RX_Funcptr();
}

// TXC interrupt handler
// for calling a specific function in case the data successfully transmitted
ISR(__vector_15)
{
	// call the function if not null
	if(UART_TX_Funcptr != 0)
		UART_TX_Funcptr();
}
