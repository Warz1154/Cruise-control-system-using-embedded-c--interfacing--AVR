/*
 * ADC.c
 *
 *  Created on: Dec 12, 2022
 *      Author: Ahmed
 */

#include "ADC.h"

u8_t ADC_Mode_Buffer;
u8_t ADC_TriggerMode_Buffer;

u16_t* gu16_ADC_reading;

ADC_ERROR_t ADC_Init(str_ADC_config_t*confg_s)
{
	switch(confg_s->u8_voltage)
	{
		case INTERNAL_VREF_OFF:
			CLEAR_BIT(ADMUX, REFS0);
			CLEAR_BIT(ADMUX, REFS1);
			break;

		case AVCC_EXTERNAL_CAPACITOR_AREF_PIN:
			SET_BIT(ADMUX, REFS0);
			CLEAR_BIT(ADMUX, REFS1);
			break;

		case INTERNAL_VREF_2_56_EXTERNAL_INTERNAL:
			SET_BIT(ADMUX, REFS0);
			SET_BIT(ADMUX, REFS1);
			break;

		default:
			return ADC_E_NOK;
	}

	switch(confg_s->u8_ADC_Channel)
	{
		case CHANNEL_ADC0:
			CLEAR_BIT(ADMUX, MUX0);
			CLEAR_BIT(ADMUX, MUX1);
			CLEAR_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC1:
			SET_BIT(ADMUX, MUX0);
			CLEAR_BIT(ADMUX, MUX1);
			CLEAR_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC2:
			CLEAR_BIT(ADMUX, MUX0);
			SET_BIT(ADMUX, MUX1);
			CLEAR_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			break;

		case CHANNEL_ADC3:
			SET_BIT(ADMUX, MUX0);
			SET_BIT(ADMUX, MUX1);
			CLEAR_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC4:
			CLEAR_BIT(ADMUX, MUX0);
			CLEAR_BIT(ADMUX, MUX1);
			SET_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC5:
			SET_BIT(ADMUX, MUX0);
			CLEAR_BIT(ADMUX, MUX1);
			SET_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC6:
			CLEAR_BIT(ADMUX, MUX0);
			SET_BIT(ADMUX, MUX1);
			SET_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		case CHANNEL_ADC7:
			SET_BIT(ADMUX, MUX0);
			SET_BIT(ADMUX, MUX1);
			SET_BIT(ADMUX, MUX2);
			CLEAR_BIT(ADMUX, MUX3);
			CLEAR_BIT(ADMUX, MUX4);
			break;

		default:
			return ADC_E_NOK;
	}

	switch(confg_s->u8_ADC_prescaler)
	{
		case ADC_PRESCALER_0:
			CLEAR_BIT(ADCSRA, ADPS0);
			CLEAR_BIT(ADCSRA, ADPS1);
			CLEAR_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_2:
			SET_BIT(ADCSRA, ADPS0);
			CLEAR_BIT(ADCSRA, ADPS1);
			CLEAR_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_4:
			CLEAR_BIT(ADCSRA, ADPS0);
			SET_BIT(ADCSRA, ADPS1);
			CLEAR_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_8:
			SET_BIT(ADCSRA, ADPS0);
			SET_BIT(ADCSRA, ADPS1);
			CLEAR_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_16:
			CLEAR_BIT(ADCSRA, ADPS0);
			CLEAR_BIT(ADCSRA, ADPS1);
			SET_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_32:
			SET_BIT(ADCSRA, ADPS0);
			CLEAR_BIT(ADCSRA, ADPS1);
			SET_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_64:
			CLEAR_BIT(ADCSRA, ADPS0);
			SET_BIT(ADCSRA, ADPS1);
			SET_BIT(ADCSRA, ADPS2);
			break;

		case ADC_PRESCALER_128:
			SET_BIT(ADCSRA, ADPS0);
			SET_BIT(ADCSRA, ADPS1);
			SET_BIT(ADCSRA, ADPS2);
			break;

		default:
			return ADC_E_NOK;
	}

	switch(confg_s->u8_ADC_Adjustment)
	{
		case LEFT_ADJUSTMENT:
			SET_BIT(ADMUX, ADLAR);
			break;

		case RIGHT_ADJUSTMENT:
			CLEAR_BIT(ADMUX, ADLAR);
			break;

		default:
			return ADC_E_NOK;
	}

	switch(confg_s->u8_ADC_Mode)
	{
		case ADC_INTERRUPT_MODE:
			// Enable Global interrupt
			GLOBALInt_enable();
			// ADC Interrupt Enable
			SET_BIT(ADCSRA, ADIE);
			break;

		case ADC_POLLING_MODE:
			if(confg_s->u8_ADC_trigger_mode != ADC_AUTO_TRIGGER_MODE_FREE_RUNNING)
				return ADC_E_NOK;
			break;

		default:
			return ADC_E_NOK;
	}

	switch(confg_s->u8_ADC_trigger_mode)
	{
		case ADC_AUTO_TRIGGER_MODE_FREE_RUNNING:
			CLEAR_BIT(SFIOR, ADTS0);
			CLEAR_BIT(SFIOR, ADTS1);
			CLEAR_BIT(SFIOR, ADTS2);
			break;

		case ADC_AUTO_TRIGGER_MODE_EXT_INT0:
			CLEAR_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			CLEAR_BIT(SFIOR, ADTS2);

			// enable EXT INT0
			EXTInt_enable(EXT_INT0); // The ADC conversion will start at the rising edge of EXT INT0

			// Set the external interrupt as rising edge
			EXTInt_Mode(EXT_INT0, RISING_EDGE);

			// Setting PIND2 as input
			CLEAR_BIT(DDRD, 2);
			// Setting the internal resistor as pull down
			CLEAR_BIT(PORTD, 2);
			break;

		// what should i do here ? should i initialize the timer? on which pre-scaler? and how to to configure timer compare while my timer supports overflow only?
		// this case is not working, use ovf timer instead
		case ADC_AUTO_TRIGGER_MODE_TIMER0_CMP:
			SET_BIT(SFIOR, ADTS0);
			SET_BIT(SFIOR, ADTS1);
			CLEAR_BIT(SFIOR, ADTS2);

			// TIMER0 AS CTC MODE
			CLEAR_BIT(TCCR0, WGM00);
			SET_BIT(TCCR0, WGM01);

			// TIMER0 CTC Interrupt enable
			SET_BIT(TIMSK, OCIE0);

			// INIT TIMER COUNTER REGISTER
			TCNT0 = 0;
			// compare match at the overflow
			OCR0 = 128;

			// Starting timer0
			// TIMER0 CTC PRE-SCALER F_CPU
			SET_BIT(TCCR0, CS00);
			CLEAR_BIT(TCCR0, CS01);
			CLEAR_BIT(TCCR0, CS02);
			break;

		case ADC_AUTO_TRIGGER_MODE_TIMER0_OVF:
			CLEAR_BIT(SFIOR, ADTS0);
			CLEAR_BIT(SFIOR, ADTS1);
			SET_BIT(SFIOR, ADTS2);

			Str_TimerConfiguration_t confg;
			confg.Interrupt_Mode = INTERRUPT;
			confg.Timer_Channel = TIMER0;
			confg.Timer_Mode = TIMER_MODE;
			confg.Timer_Psc = F_CPU_CLOCK_TIMER_0;

			Timer_Init(&confg);
			Timer_Start(TIMER0, TIMER0_OVERFLOW);
			break;

		default:
			return ADC_E_NOK;
	}

	if(confg_s->u8_ADC_trigger_mode != ADC_AUTO_TRIGGER_MODE_FREE_RUNNING)
		SET_BIT(ADCSRA, ADATE);

	// Everything is fine then enable the ADC
	SET_BIT(ADCSRA, ADEN);
	// internal buffer for adc mode (polling, interrupt)
	ADC_Mode_Buffer = confg_s->u8_ADC_Mode;
	// internal buffer for ADC Trigger Mode
	ADC_TriggerMode_Buffer = confg_s->u8_ADC_trigger_mode;

	return ADC_E_OK;
}


ADC_ERROR_t ADC_startConversion()
{
	switch(ADC_TriggerMode_Buffer)
	{
		case ADC_AUTO_TRIGGER_MODE_FREE_RUNNING:
			// starting conversion
			SET_BIT(ADCSRA, ADSC);
			switch(ADC_Mode_Buffer)
			{
				case ADC_POLLING_MODE:
					// wait until conversion is completed
					while(!GET_BIT(ADCSRA, ADIF));	// polling on conversion to be completed
					SET_BIT(ADCSRA, ADIF); // Clearing the ADC interrupt flag by setting logical one to it
					break;
			}
			break;

		default:
			return ADC_E_NOK;
	}
	return ADC_E_OK;
}

void ADC_readChannel(u16_t* reading)
{
	*reading = ADC; // reading the ADC value
}
