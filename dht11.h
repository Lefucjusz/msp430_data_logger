/*
 * dht11.h
 *
 *  Created on: 5 mar 2020
 *      Author: Lefucjusz
 */
#ifndef DHT11_H_
#define DHT11_H_

#include <stdint.h>

#define DHT11_ERROR 0xFF

typedef struct
{
	uint8_t humidity;
	uint8_t temperature;
} dht11_t;

dht11_t dht11_get_measurements(void);

#endif /* DHT11_H_ */
