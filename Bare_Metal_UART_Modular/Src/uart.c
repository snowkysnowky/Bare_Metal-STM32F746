/*
 * uart.c
 *
 *  Created on	: Jul 8, 2022
 *      Author	: DAW
 *  	Updated : Jil 19,2022
 */

#include "uart.h"

#define	GPIOAEN			(1U<<0)
#define	GPIOBEN			(1U<<1)
#define USART1EN		(1U<<4)

#define CR1_TE			(1U<<3)
#define CR1_TXEIE		(1U<<7)
#define CR1_RE			(1U<<2)
#define CR1_RXNEIE		(1U<<5)
#define CR1_UE			(1U<<0)


#define SYS_FREQ		(16000000)
#define APB2_CLK		SYS_FREQ

#define UART_BAUDRATE 	(115200)

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t BaudRate);

void uart1_write(int ch);

int __io_putchar(int ch)
{
	uart1_write(ch);
	return ch;
}

void usart1_tx_init(void)
{
	/***************Configure uart gpio pin*****************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;

	/*Set PA9 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set PA9 alternate function type to UART_TX (AF8)*/
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	/***************Configure uart module***************/
	/*Enable clock access to uart6*/
	RCC->APB2ENR |=USART1EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART1->CR1 = CR1_TE;//transmit enable

	/*Enable uart module*/
	USART1->CR1 |=CR1_UE;//uart enable

}

void usart1_rxtx_init(void)
{
	/***************Configure uart gpio pin*****************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;
	RCC->AHB1ENR |=GPIOBEN;

	/*Set PA9 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set PA9 alternate function type to UART_TX (AF8)*/
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	/*Set PB7 mode to alternate function mode*/
	GPIOB->MODER  &=~(1U<<14);
	GPIOB->MODER  |= (1U<<15);

	/*Set PB7 alternate function type to UART_TX (AF7)*/
	GPIOB->AFR[0] |= (1U<<28);
	GPIOB->AFR[0] |= (1U<<29);
	GPIOB->AFR[0] |= (1U<<30);
	GPIOB->AFR[0] &=~(1U<<31);

	/***************Configure uart module***************/
	/*Enable clock access to uart1*/
	RCC->APB2ENR |= USART1EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART1->CR1 = (CR1_TE | CR1_RE);//transmit recive enable

	/*Enable uart module*/
	USART1->CR1 |=CR1_UE;//uart enable

}

void usart1_rx_interrupt_init(void)
{
	/***************Configure uart gpio pin*****************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;
	RCC->AHB1ENR |=GPIOBEN;

	/*Set PA9 mode to alternate function mode*/
//	GPIOA->MODER &=~(1U<<18);
//	GPIOA->MODER |= (1U<<19);

	/*Set PA9 alternate function type to UART_TX (AF8)*/
//	GPIOA->AFR[1] |= (1U<<4);
//	GPIOA->AFR[1] |= (1U<<5);
//	GPIOA->AFR[1] |= (1U<<6);
//	GPIOA->AFR[1] &=~(1U<<7);

	/*Set PB7 mode to alternate function mode*/
	GPIOB->MODER  &=~(1U<<14);
	GPIOB->MODER  |= (1U<<15);

	/*Set PB7 alternate function type to UART_RX (AF7)*/
	GPIOB->AFR[0] |= (1U<<28);
	GPIOB->AFR[0] |= (1U<<29);
	GPIOB->AFR[0] |= (1U<<30);
	GPIOB->AFR[0] &=~(1U<<31);

	/***************Configure uart module***************/
	/*Enable clock access to uart1*/
	RCC->APB2ENR |= USART1EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART1->CR1 = CR1_RE;// recive enable

	/*Enable RXNE interrupt*/
	USART1->CR1 |= CR1_RXNEIE;

	/*Enable Usart1 interrupt in NVIC*/
	NVIC_EnableIRQ(USART1_IRQn);

	/*Enable uart module*/
	USART1->CR1 |=CR1_UE;//uart enable

}

void usart1_tx_interrupt_init(void)
{
	/***************Configure uart gpio pin*****************/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;
//	RCC->AHB1ENR |=GPIOBEN;

	/*Set PA9 mode to alternate function mode*/
	GPIOA->MODER &=~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*Set PA9 alternate function type to UART_TX (AF8)*/
	GPIOA->AFR[1] |= (1U<<4);
	GPIOA->AFR[1] |= (1U<<5);
	GPIOA->AFR[1] |= (1U<<6);
	GPIOA->AFR[1] &=~(1U<<7);

	/*Set PB7 mode to alternate function mode*/
//	GPIOB->MODER  &=~(1U<<14);
//	GPIOB->MODER  |= (1U<<15);

	/*Set PB7 alternate function type to UART_RX (AF7)*/
//	GPIOB->AFR[0] |= (1U<<28);
//	GPIOB->AFR[0] |= (1U<<29);
//	GPIOB->AFR[0] |= (1U<<30);
//	GPIOB->AFR[0] &=~(1U<<31);

	/***************Configure uart module***************/
	/*Enable clock access to usart1*/
	RCC->APB2ENR |= USART1EN;

	/*Configure baudrate*/
	uart_set_baudrate(USART1, APB2_CLK, UART_BAUDRATE);

	/*Configure the transfer direction*/
	USART1->CR1 = CR1_TE;//transmit enable

	/*Enable RXNE interrupt*/
//	USART1->CR1 |= CR1_TXEIE;


	/*Enable Usart1 interrupt in NVIC*/
	NVIC_EnableIRQ(USART1_IRQn);

	/*Enable uart module*/
	USART1->CR1 |=CR1_UE;//uart enable

}

char uart1_read(void)
{
	/*Make sure the recive data register is not empty*/
	while(!(USART1->ISR & ISR_RXNE)){}

	/*Read data*/
	return USART1->RDR;
}

void uart1_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART1->ISR & ISR_TXE)){}

	/*Write to transmit data register*/
	USART1->TDR = (ch & 0XFF);
}

static void uart_set_baudrate(USART_TypeDef *USARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR = compute_uart_baudrate(PeriphClk,BaudRate);
}

static uint16_t compute_uart_baudrate(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate);
}

void uart_write_it(data)
{
	USART1->TDR = (ch & 0XFF);
}
