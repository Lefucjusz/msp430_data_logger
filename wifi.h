/*
 * wifi.h
 *
 *  Created on: 10 mar 2020
 *      Author: Lefucjusz
 */

#ifndef WIFI_H_
#define WIFI_H_

#include <stdint.h>
#include "bool.h"
#include "dht11.h"
#include "bmp280.h"

#define FAILURE FALSE
#define SUCCESS TRUE

uint8_t wifi_send_data_frame(dht11_t, bmp280_t);

#endif /* WIFI_H_ */
