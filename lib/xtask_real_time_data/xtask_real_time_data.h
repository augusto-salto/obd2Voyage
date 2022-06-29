#ifndef XTASK_REAL_TIME_DATA_H
#define XTASK_REAL_TIME_DATA_H



#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>


#include "interface_com.h"

extern ElmComm elm;

void xTask_real_time( void *pvParameters );
void vTask_real_time_start();



#endif