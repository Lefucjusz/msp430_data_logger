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

void usart1_putc(char);
void usart1_puts(const char*);

char usart1_getc(void);
void usart1_gets(char*);

void usart1_put_s32(int32_t);


#endif /* UART_H_ */
