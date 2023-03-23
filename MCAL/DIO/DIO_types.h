/*
 * DIO_types.h
 *
 *  Created on: Dec 2, 2022
 *      Author: Abdelrahman
 */

#ifndef MCAL_DIO_DIO_TYPES_H_
#define MCAL_DIO_DIO_TYPES_H_


//PINS
typedef enum
{
	 PIN0,
	 PIN1,
	 PIN2,
	 PIN3,
	 PIN4,
	 PIN5,
	 PIN7
}DIO_PIN_ID;


//PORTS
typedef enum
{
	DPORTA,
	DPORTB,
	DPORTC,
	DPORTD
}DIO_PORT_ID;


//DIRECTIONS
typedef enum
{
	INPUT,
	OUTPUT
}DIO_DIRECTION;


//HIGH AND LOW

typedef enum
{
	LOW,
	HIGH
}DIO_VALUE;


//error 0 ok,  1 not ok

typedef enum ERROR_T
{
	E_OK,
	E_N_OK
} Error_t;


#endif /* MCAL_DIO_DIO_TYPES_H_ */
