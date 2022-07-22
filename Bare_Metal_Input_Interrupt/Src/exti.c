/*
 * exti.c
 *
 *  Created on: Jul 21, 2022
 *      Author: DAW
 */

#include "exti.h"

#define GPIOIEN		(1U<<8)
#define SYSCFGEN	(1U<<14)

void PI11_exti_init(void)
{
	/*Disable global interrupt*/
	__disable_irq();

	/*Enable clock access for GPIOI*/
	RCC->AHB1ENR |= GPIOIEN;

	/*Enable clock access to SYSCFG*/
	RCC->APB2ENR |= SYSCFGEN;

	/*Set PI11 as Input*/
	GPIOI->MODER &=~(1U<<22);
	GPIOI->MODER &=~(1U<<22);

	/*Select  PORTI for EXTI11*/
	SYSCFG->EXTICR[2] |= (1U<<15);

	/*Unmask EXTI11*/
	EXTI->IMR |= (1U<<11);

	/*Select falling edge trigger*/
	EXTI->FTSR |= (1U<<11);

	/*Enable EXTI11 line in NVIC*/
	NVIC_EnableIRQ(EXTI15_10_IRQn);

	/*Enable global interrupt*/
	__enable_irq();

}
