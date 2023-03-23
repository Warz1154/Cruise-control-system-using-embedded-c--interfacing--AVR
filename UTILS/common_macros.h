/*
 * common_macros.h
 *
 *  Created on: Oct 29, 2022
 *      Author: Ahmed
 */

#ifndef COMMON_MACROS_H_
#define COMMON_MACROS_H_

#define SET_BIT(REG, BIT_NO) 		(REG |= (1<<BIT_NO))
#define	CLEAR_BIT(REG, BIT_NO)		(REG &= ~(1<<BIT_NO))
#define GET_BIT(REG, BIT_NO)		((REG & (1<<BIT_NO)) >> BIT_NO)
#define TOGGLE_BIT(REG, BIT_NO)		(REG ^= (1<<BIT_NO))

#endif /* COMMON_MACROS_H_ */
