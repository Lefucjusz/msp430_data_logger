/*
 * wifi.c
 *
 *  Created on: 10 mar 2020
 *      Author: Lefucjusz
 *
 */
#include "wifi.h"
#include "uart.h"

void wifi_send_data_frame(dht11_t dht11, bmp280_t bmp280)
{
	usart1_puts("GET /log?temp1="); //Send GET request
	usart1_put_s32(dht11.temperature);
	usart1_puts("&humid=");
	usart1_put_s32(dht11.humidity);
	usart1_puts("&temp2=");
	usart1_put_s32(bmp280.temperature);
	usart1_puts("&press=");
	usart1_put_s32(bmp280.pressure);
	usart1_puts(" HTTP/1.0\r\n\r\n");
}
