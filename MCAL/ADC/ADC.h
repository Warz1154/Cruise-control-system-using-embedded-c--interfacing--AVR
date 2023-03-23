/*
 * adc.h
 *
 *  Created on: Jan 9, 2020
 *      Author: H
 */

#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_
/*****************************************************************************
 *___________________________INCLUDES__________________________________________

 * ***************************************************************************/
#include "../../UTILS/common_macros.h"
#include "../../UTILS/types.h"
#include "../TIMER/TIMER_INTERFACE.h"
#include "../EXT_INT/interrupt.h"
#include "avr/interrupt.h"

/******************************************************************************
 *__________________________________TYPEDEF____________________________________

 * ****************************************************************************/

typedef struct
{
	u8_t u8_voltage;
	u8_t u8_ADC_Adjustment;
	u8_t u8_ADC_Channel;
	u8_t u8_ADC_prescaler;
	u8_t u8_ADC_Mode;
	u8_t u8_ADC_trigger_mode;
}str_ADC_config_t;

/***************************************************************************
 * ___________________________EXTERN_______________________________________
 ***************************************************************************/



/*****************************************************************************
 * _________________________LOCAL_MACROS______________________________________
 *
 ****************************************************************************/
#define INTERNAL_VREF_OFF 0
#define INTERNAL_VREF_2_56_EXTERNAL_INTERNAL 3
#define AVCC_EXTERNAL_CAPACITOR_AREF_PIN 1

#define CHANNEL_ADC0 0
#define CHANNEL_ADC1 1
#define CHANNEL_ADC2 2
#define CHANNEL_ADC3 3
#define CHANNEL_ADC4 4
#define CHANNEL_ADC5 5
#define CHANNEL_ADC6 6
#define CHANNEL_ADC7 7

#define ADC_PRESCALER_0 0
#define ADC_PRESCALER_2 1
#define ADC_PRESCALER_4 2
#define ADC_PRESCALER_8 3
#define ADC_PRESCALER_16 4
#define ADC_PRESCALER_32 5
#define ADC_PRESCALER_64 6
#define ADC_PRESCALER_128 7


#define LEFT_ADJUSTMENT 1
#define RIGHT_ADJUSTMENT 0

#define ADC_INTERRUPT_MODE 0
#define ADC_POLLING_MODE 1

#define ADC_AUTO_TRIGGER_MODE_FREE_RUNNING 0
#define ADC_AUTO_TRIGGER_MODE_EXT_INT0 2
#define ADC_AUTO_TRIGGER_MODE_TIMER0_CMP 3
#define ADC_AUTO_TRIGGER_MODE_TIMER0_OVF 4
//#define ADC_AUTO_TRIGGER_MODE_TIMER0_ICU 7

typedef enum
{
	ADC_E_NOK,
	ADC_E_OK
}ADC_ERROR_t;

/******************************************************************************
 * _______________________FUNCTION_PROTOTYPES__________________________________
 *
 ******************************************************************************/

// ADC initialization
ADC_ERROR_t ADC_Init(str_ADC_config_t*confg_s);

/***************************************************************************
 * ________________________Starting ADC Conversion__________________________
 * User can use this function in case of Free Running trigger mode only
 * This function will start converting the ADC signal from analog to digital
 ****************************************************************************/
ADC_ERROR_t ADC_startConversion();

/****************************************************************************
 * _____________________Reading The ADC converted value______________________
 * User can use this function to read the ADC value after
 * the conversion is completed.
 * If Free Running mode is selected
 * 		--> if polling mode is selected
 * 				-->use this function after the startConversion function
 * 					by one line
 * 		--> if interrupt mode is selected
 * 				--> use this function in ADC interrupt ISR handler
 * If any other interrupt trigger mode is selected
 * 		--> use this function in ADC Interrupt ISR handler
 *****************************************************************************/
void ADC_readChannel(u16_t* reading);

#endif /* MCAL_ADC_ADC_H_ */
