#include "xtask_display.h"

HardwareSerial NextionSerial(2);

void xTask_display( void *pvParameters )
{

    //NextionSerial.setPins(16,17);
   // NextionSerial.begin(9600, SERIAL_8N1, 16, 17);
    EasyNex myNex(NextionSerial);
    myNex.begin(9600);
    myNex.writeStr("page page1");
    vTaskDelay(pdMS_TO_TICKS(2000));
    myNex.writeStr("page page2");

    while (car.is_connecting() || !car.is_running())
    {
        
        vTaskDelay(1000);
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nLOOP PAGE2");
        xSemaphoreGive(xSerial_semaphore);
    }

    myNex.writeStr("page0");
    


    while (1)
    {
        myNex.writeNum("z0.val", car.get_rpm());
        vTaskDelay(pdMS_TO_TICKS(50));
        myNex.writeNum("z1.val", car.get_speed());
        vTaskDelay(pdMS_TO_TICKS(50));
        myNex.writeStr("t0.txt", (String) car.get_temp_01());
        vTaskDelay(pdMS_TO_TICKS(50)); 
         myNex.writeStr("t1.txt", (String) car.get_batery_volt());
        vTaskDelay(pdMS_TO_TICKS(50));     
    }
    

}



void vTask_display_start()
{
    xTaskCreate( xTask_display /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_display_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 2000 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 1 /* Prioridade */
                , NULL ); /* Handle da tarefa, opcional  */
}