#include "main.h"



void setup()
{

xSerial_semaphore = xSemaphoreCreateMutex();
xHardwareSerial_semaphore = xSemaphoreCreateMutex();
xBle_semaphore = xSemaphoreCreateMutex();

    Serial.begin(115200);
    ble_client_setup();
}

void loop()
{
    ble_client_loop();
    delay(500);
   

}