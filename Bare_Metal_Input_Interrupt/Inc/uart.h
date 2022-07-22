/*
 * uart.h
 *
 *  Created on: Jul 8, 2022
 *      Author: DAW
 */
#ifndef UART_H_
#define UART_H_
#include <stdint.h>

#include "stm32f7xx.h"

void usart1_tx_init(void);
char uart1_read(void);
void usart1_rxtx_init(void);
void usart1_rx_interrupt_init(void);

#define ISR_RXNE		(1U<<5)


#endif /* UART_H_ */
