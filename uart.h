/*
 * uart.h
 *
 *  Created on: 6 mar 2020
 *      Author: Lefucjusz
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>

void usart1_init_9600(void);
void usart1_putchar(uint8_t byte);
void usart1_string(const char* str);

#endif /* UART_H_ */
