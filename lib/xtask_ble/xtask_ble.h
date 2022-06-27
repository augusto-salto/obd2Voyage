#ifndef XTASK_BLE_H
#define XTASK_BLE_H


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>
#include "bluetooth_ble.h"
#include "xtask_real_time_data.h"

extern TaskHandle_t handle_real_time_data;
extern Car car;

void xTask_ble( void *pvParameters );
void vTask_ble_start();




#endif
