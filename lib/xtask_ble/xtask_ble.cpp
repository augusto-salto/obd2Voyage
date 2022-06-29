#include "xtask_ble.h"

TaskHandle_t handle_ble;


void xTask_ble( void *pvParameters )
{
                                //ble_client_setup();
    vTaskDelay(pdMS_TO_TICKS(1000));
    bool flagRealTimeIsRunning = false;

    while (1)
    {
                                //ble_client_loop();
        

        if(car.is_running() && flagRealTimeIsRunning == false)
        {
            flagRealTimeIsRunning = true;
            vTask_real_time_start();
            
        }else if(flagRealTimeIsRunning)
        {
            vTaskDelete(handle_real_time_data);
             flagRealTimeIsRunning = false;
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    

}



void vTask_ble_start()
{
    xTaskCreate( xTask_ble /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_ble_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 4000 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 10 /* Prioridade */
                , &handle_ble ); /* Handle da tarefa, opcional  */
}