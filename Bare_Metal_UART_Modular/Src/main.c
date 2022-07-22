#include <stdio.h>
#include <stdint.h>
#include "stm32f7xx.h"
#include "uart.h"

#define GPIOIEN		(1U<<8)
#define GPIOI_1		(1U<<1)
#define LED_PIN		(GPIOI_1)

//static void uart_callback(void);
char helo[10]= "HALLO";
char key;

int main(void)
{
	/*Enable clock GPIOI*/
	RCC->AHB1ENR |= GPIOIEN;

	/*Set PI1 as output pin*/
	GPIOI->MODER |= (1U<<2);
	GPIOI->MODER &=~(1U<<3);

//	usart1_rx_interrupt_init();
//	usart1_rxtx_init();
	usart1_tx_interrupt_init();
	uart1_write(helo);

	while(1)
	{
//		key = USART1->RDR;
//		if(key == '1')
//		{
//			GPIOI->ODR |= LED_PIN;
//		}
//		else
//		{
//			GPIOI->ODR &=~LED_PIN;
//		}
	}

}

//static void uart_callback(void)
//{
//	key = USART1->RDR;
//	if(key == '1')
//	{
//		GPIOI->ODR |= LED_PIN;
//	}
//	else
//	{
//	GPIOI->ODR &=~LED_PIN;
//
//	}
//}

static void uart_callback(void)
{
	uart1_write(helo);
	for(int i = 0;i<10000;i++){}

}

void USART1_IRQHandler(void)
{
	/*check if RXNE is set*/
	if(USART1->ISR & ISR_TXE)
	{
		//do something
		uart_callback();
	}
}

//void USART1_IRQHandler(void)
//{
//	/*check if RXNE is set*/
//	if(USART1->ISR & ISR_RXNE)
//	{
//		//do something
//		uart_callback();
//	}
//}





