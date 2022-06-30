#ifndef TASK_DISPLAY_H
#define TASK_DISPLAY_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <Arduino.h>

#include "interface_com.h"

extern ElmComm elm;

void xTask_display( void *pvParameters );
void vTask_display_start();








#endif