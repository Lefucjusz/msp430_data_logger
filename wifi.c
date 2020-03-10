/*
 * wifi.c
 *
 *  Created on: 10 mar 2020
 *      Author: Lefucjusz
 */
#include "wifi.h"
#include "uart.h"
#include <string.h>

uint8_t wifi_at_mode_start(void)
{
	char response[4];
	usart1_send_string("+++");
	response[0] = usart1_getchar();
	if(response[0] != 'a')
	{
		return 0;
	}
	usart1_putchar('a');
	usart1_get_string(response);
	if(strcmp(response, "+ok"))
	{
		return 0;
	}
	return 1;

}

static void wifi_at_mode_stop(void)
{

}

static void wifi_transparent_mode_start(void)
{

}

static void wifi_transparent_mode_stop(void)
{

}

static void wifi_sta_init(void)
{

}

static void wifi_http_init(void)
{

}
