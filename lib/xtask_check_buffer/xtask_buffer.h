#ifndef XTASK_CHECK_BUFFER_H
#define XTASK_CHECK_BUFFER_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>

#include "interface_com.h"



void xTask_check_buffer( void *pvParameters );
void vTask_check_buffer_start();



extern ElmComm elm;










#endif