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

void usart1_putchar(char);
void usart1_send_string(const char*);

char usart1_getchar(void);
void usart1_get_string(char*);

#endif /* UART_H_ */
