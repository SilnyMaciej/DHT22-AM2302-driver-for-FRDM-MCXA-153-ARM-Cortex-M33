/*
 * dht22_driver.h
 *
 *  Created on: 6 maj 2026
 *      Author: silnymaciej
 */

#ifndef DHT22_DRIVER_H_
#define DHT22_DRIVER_H_

#include "pin_mux.h"
#include "board.h"
#include "delay_us.h"

#define DHT22_GPIO BOOT_INIT_DHT22_DHT22_GPIO
#define DHT22_PIN_MASK BOOT_INIT_DHT22_DHT22_GPIO_PIN_MASK
#define DHT22_TIMEOUT_CYCLES 100000U

typedef enum{
	GND_ERROR = 112,
	VCC_ERROR = 445,
	CHECK_SUM_ERROR = 554
}g_Sensor_Check_t;


uint32_t DHT22_Get_Temperature_And_RH(void);


#endif /* DHT22_DRIVER_H_ */
