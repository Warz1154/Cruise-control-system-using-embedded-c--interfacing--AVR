/*
 * ultrasonic.h
 *
 *  Created on: Dec 15, 2022
 *      Author: Ahmed
 */

#ifndef HAL_ULTRASONIC_ULTRASONIC_H_
#define HAL_ULTRASONIC_ULTRASONIC_H_

#include "../../MCAL/DIO/DIO.h"
#include "../../SERVICE/ICU/ICU.h"
#include <util/delay.h>

// Error handling defines
#define US_E_NOK 	0
#define US_E_OK 	1
#define US_ERROR_t	u8_t

// CHANNELS DEFINES
#define US_PORT			DIO_PORT_ID
#define US_PIN			DIO_PIN_ID
#define US_CHANNEL		EXTINT_Channel_t
#define US_CHANNEL_0	EXT_INT0
#define US_CHANNEL_1 	EXT_INT1
#define US_CHANNEL_2 	EXT_INT2


// Ultrasonic constants
#define US_CALIBRATION_FACTOR 58  // this is the inverse of the speed in microSec/cm divided by 2 which is half the time need for the sound to return to the ultrasonic

// Ultrasonic config structure
typedef struct
{
	US_PORT Trigger_Port;
	US_PIN Trigger_Pin;
	US_CHANNEL Echo_Channel;
	u8_t distance;
}str_US_config_t;


/*_____________________Ultrasonic Initialization______________*
 * Description	: Initalize the ultrasonic with the ICU
 * Input		: Structure of ultrasonic configurations
 * Output	  	: Error checking
 *____________________________________________________________*/
US_ERROR_t ULTRASONIC_Init(str_US_config_t *confg_s);

/*_____________________Ultrasonic Read Distance_______________*
 * Description	: Read captured distance by the ultrasonic
 * Input		: Echo channel (EXT_INT0, EXT_INT1, EXT_INT2), Pointer to a var which the result will be returned to
 * Output	  	: Error checking
 *____________________________________________________________*/
US_ERROR_t ULTRASONIC_readDistance(str_US_config_t *confg_s);

#endif /* HAL_ULTRASONIC_ULTRASONIC_H_ */




