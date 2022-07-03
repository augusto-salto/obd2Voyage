#include "xtask_ble.h"

TaskHandle_t handle_ble;
ElmComm elm;


void xTask_ble( void *pvParameters )
{

    elm.elm_setup();
    vTaskDelay(pdMS_TO_TICKS(1000));
    bool flagElmIsRunning = false;

                                

    while (1)
    {
        elm.elm_loop();
        

        if(car.is_running() && !flagElmIsRunning)
        {
            flagElmIsRunning = true;
            vTask_real_time_start();
            vTask_get_data_start();
            
        }else if(!car.is_running() && flagElmIsRunning)
        {
            vTaskDelete(handle_real_time_data);
            vTaskDelete(handle_get_data);
            flagElmIsRunning = false;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
    

}



void vTask_ble_start()
{
    xTaskCreate( xTask_ble /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_ble_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 6000 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 10 /* Prioridade */
                , &handle_ble ); /* Handle da tarefa, opcional  */
}