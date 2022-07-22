/*
 * exti.h
 *
 *  Created on: Jul 21, 2022
 *      Author: DAW
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f7xx.h"

void PI11_exti_init(void);
#define LINE11		(1U<<11)

#endif /* EXTI_H_ */
