#ifndef XTASK_GET_DATA_H
#define XTASK_GET_DATA_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>


#include "interface_com.h"

extern ElmComm elm;

void xTask_get_data( void *pvParameters );
void vTask_get_data_start();












#endif