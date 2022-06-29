#include "main.h"

String resposta = "                                                                        ";
void setup()
{

xQueue_bufferEntrada = xQueueCreate( 15, sizeof(std::string *));

xSerial_semaphore = xSemaphoreCreateMutex();
xHardwareSerial_semaphore = xSemaphoreCreateMutex();
xBle_semaphore = xSemaphoreCreateMutex();

Serial.begin(115200);
vTask_ble_start();
vTask_check_buffer_start();
   
}

void loop()
{
    
 
   vTaskDelay(pdMS_TO_TICKS(100));

}