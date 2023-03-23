/*
 * button.h
 *
 *  Created on: Dec 16, 2022
 *      Author: Ahmed
 */

#ifndef HAL_BUTTON_BUTTON_H_
#define HAL_BUTTON_BUTTON_H_

// Includes
#include "../../MCAL/DIO/DIO.h"

// Error handling defines
#define BTN_E_NOK 	0
#define BTN_E_OK 	1
#define BTN_ERROR_t	u8_t

// CHANNELS DEFINES
#define BTN_PORT		DIO_PORT_ID
#define BTN_PIN			DIO_PIN_ID

typedef enum
{
	NOT_PRESSED,
	PRESSED
}en_BTN_state_t;

typedef struct
{
	BTN_PORT Port;
	BTN_PIN Pin;
	en_BTN_state_t State;
}str_BTN_config_t;

BTN_ERROR_t BUTTON_Init(str_BTN_config_t * BTN_confg_s);
BTN_ERROR_t BUTTON_readState(str_BTN_config_t * BTN_confg_s);

#endif /* HAL_BUTTON_BUTTON_H_ */
