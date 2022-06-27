#include "main.h"



void setup()
{
    Serial.begin(115200);
    ble_client_setup();
}

void loop()
{
    ble_client_loop();
    delay(500);
    

}