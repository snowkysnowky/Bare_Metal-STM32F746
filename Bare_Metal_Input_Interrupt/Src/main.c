#include <stdio.h>
#include <stdint.h>
#include "stm32f7xx.h"
#include "uart.h"
#include "exti.h"

#define GPIOIEN		(1U<<8)
#define GPIOI_1		(1U<<1)
#define LED_PIN		(GPIOI_1)

static void exti_callback(void);


int main(void)
{

	/*Enable clock GPIOI*/
	RCC->AHB1ENR |= GPIOIEN;

	/*Set PI1 as output pin*/
	GPIOI->MODER |= (1U<<2);
	GPIOI->MODER &=~(1U<<3);

	PI11_exti_init();
	usart1_tx_init();

	while(1)
	{

	}

}

static void exti_callback(void)
{
	printf("Button pressed..\n\r");
	GPIOI->ODR ^= LED_PIN;
}

void EXTI15_10_IRQHandler(void)
{
	if((EXTI->PR & LINE11) != 0)
	{
		/*Clear PR flag*/
		EXTI->PR |= LINE11;
		/*Do something*/
		exti_callback();
	}
}





