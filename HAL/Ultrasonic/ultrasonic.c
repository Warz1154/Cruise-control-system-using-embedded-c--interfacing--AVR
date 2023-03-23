/*
 * ultrasonic.c
 *
 *  Created on: Dec 15, 2022
 *      Author: Ahmed
 */

// Includes
#include "ultrasonic.h"


US_ERROR_t ULTRASONIC_Init(str_US_config_t *confg_s)
{
	// ERROR CHECKING (GUARDS)
	if(confg_s->Trigger_Port < DPORTA || confg_s->Trigger_Port > DPORTD)
		return US_E_NOK;

	if(confg_s->Trigger_Pin < PIN0 || confg_s->Trigger_Pin > PIN7)
		return US_E_NOK;

	if(confg_s->Echo_Channel < US_CHANNEL_0 || confg_s->Echo_Channel > US_CHANNEL_2)
		return US_E_NOK;

	// Trigger pin as output
	DIO_setPin_Direction(confg_s->Trigger_Port, confg_s->Trigger_Pin, OUTPUT);

	// ECHO INIT (ICU INIT)
	ICU_Init(confg_s->Echo_Channel);

	return US_E_OK;
}

US_ERROR_t ULTRASONIC_readDistance(str_US_config_t *confg_s)
{
	// ERROR CHECKING (GUARDS)
	if(confg_s->Trigger_Port < DPORTA || confg_s->Trigger_Port > DPORTD)
		return US_E_NOK;

	if(confg_s->Trigger_Pin < PIN0 || confg_s->Trigger_Pin > PIN7)
		return US_E_NOK;

	if(confg_s->Echo_Channel < US_CHANNEL_0 || confg_s->Echo_Channel > US_CHANNEL_2)
		return US_E_NOK;

	// Triggering the ultrasonic to start sending sound waves (10+ microSec wave)
	DIO_setPin_Value(confg_s->Trigger_Port, confg_s->Trigger_Pin, HIGH);
	_delay_us(11);
	DIO_setPin_Value(confg_s->Trigger_Port, confg_s->Trigger_Pin, LOW);

	// Temp variable to store captured time
	u16_t time;
	// Reading the time captured by ICU
	ICU_readCapturedTime(&time);

	// Returning the calculated distance
	confg_s->distance = (float)time/(float)US_CALIBRATION_FACTOR;

	return US_E_OK;
}
