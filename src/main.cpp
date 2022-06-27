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

    

   if(car.is_connecting())
   {
    Serial.print("\nCONECTANDO...");
   }else if(!car.is_running())
   {
    Serial.print("\nVEICULO NAO ENCONTRADO!");
   }else 

   if(car.is_running())
   {
        ble_send_pid(SERVICE_01, RPM_ENGINE, "1");
   }

   if(car.is_connecting() || !car.is_running())
   {
    ble_send_pid(SERVICE_01, SUPORTED_PIDS_01_20, "1"); // TESTE DE COMUNICAÇÃO
   }

   
    delay(200);

}