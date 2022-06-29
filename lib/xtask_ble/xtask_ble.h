#ifndef XTASK_BLE_H
#define XTASK_BLE_H


#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>

#include "interface_com.h"

#include "xtask_real_time_data.h"
#include "xtask_get_data.h"
#include "xtask_buffer.h"

extern TaskHandle_t handle_real_time_data;
extern TaskHandle_t handle_get_data;
extern TaskHandle_t handle_check_buffer;

extern Car car;

void xTask_ble( void *pvParameters );
void vTask_ble_start();





#endif
