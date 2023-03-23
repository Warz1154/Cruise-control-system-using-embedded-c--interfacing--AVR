/*
 * button.c
 *
 *  Created on: Dec 16, 2022
 *      Author: Ahmed
 */

#include "button.h"

BTN_ERROR_t BUTTON_Init(str_BTN_config_t * BTN_confg_s)
{
	return DIO_setPin_Direction(BTN_confg_s->Port, BTN_confg_s->Pin, INPUT);
}

BTN_ERROR_t BUTTON_readState(str_BTN_config_t * BTN_confg_s)
{
	u8_t temp, temp2;
	temp2 = DIO_getPin_Value(BTN_confg_s->Port, BTN_confg_s->Pin, &temp);
	BTN_confg_s->State = temp;
	return temp2;
}
