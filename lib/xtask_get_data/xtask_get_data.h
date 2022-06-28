#ifndef XTASK_GET_DATA_H
#define XTASK_GET_DATA_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>
#include "bluetooth_ble.h"

extern Car car;

void xTask_get_data( void *pvParameters );
void vTask_get_data_start();












#endif