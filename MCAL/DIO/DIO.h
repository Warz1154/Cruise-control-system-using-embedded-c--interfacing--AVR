#ifndef DIO_H_
#define DIO_H_


//includes
#include "../../UTILS/types.h"
#include "../../UTILS/MCU.h"
#include "../../UTILS/common_macros.h"
#include "DIO_types.h"

//DIO.h

// PORT FUNCTIONS

//SET PORT DIRECTION

Error_t DIO_setPort_Direction(DIO_PORT_ID port_ID, DIO_DIRECTION direction); //

//SET PORT VALUE

Error_t DIO_setPort_Value(DIO_PORT_ID port_ID, u8_t value); //

//get port value

Error_t GetPort_Value(DIO_PORT_ID port_ID, u8_t *PtrPortValue);

//ACTIVE PULLUP

Error_t DIO_portActive_Pullup(u8_t port_name);


// ---------------------------------------------------------------------------------------------------------------------------------------------------

//PINS FUNCTIONS


//SET PIN DIRECTION

Error_t DIO_setPin_Direction(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, DIO_DIRECTION direction); //

//SET PIN VALUE

Error_t DIO_setPin_Value(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, DIO_VALUE value);

//GET PIN VALUE

Error_t DIO_getPin_Value(DIO_PORT_ID port_ID , DIO_PIN_ID pin_ID, u8_t *PtrPinValue);



//ACTIVATE PIN PULLUP

Error_t DIO_pinActivate_Pullup(u8_t Pin_Number);


#endif /* DIO_H_ */
