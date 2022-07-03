#ifndef MAIN_H
#define MAIN_H


#include "pid.h"
#include "bluetooth_ble.h"
#include "Arduino.h"
#include "car.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "interface_com.h"

// TAREFAS
#include "xtask_real_time_data.h"
#include "xtask_ble.h"
#include "xtask_buffer.h"
#include "xtask_display.h"

//FILAS
QueueHandle_t xQueue_bufferEntrada;


// SEMAFOROS
SemaphoreHandle_t xSerial_semaphore;
SemaphoreHandle_t xHardwareSerial_semaphore;
SemaphoreHandle_t xBle_semaphore;

//Handle
extern TaskHandle_t handle_real_time_data;
extern TaskHandle_t handle_ble;

//extern Car car;

#endif