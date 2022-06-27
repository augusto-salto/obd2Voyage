#ifndef XTASK_REAL_TIME_DATA_H
#define XTASK_REAL_TIME_DATA_H



#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>
#include "bluetooth_ble.h"

extern Car car;

void xTask_real_time( void *pvParameters );
void vTask_real_time_start();



#endif