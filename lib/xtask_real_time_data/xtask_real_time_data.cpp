#include "xtask_real_time_data.h"

TaskHandle_t handle_real_time_data;


void xTask_real_time( void *pvParameters )
{

    while (1)
    {
       elm.requestData();
        vTaskDelay(pdMS_TO_TICKS(50));
    }
    

}



void vTask_real_time_start()
{
    xTaskCreate( xTask_real_time /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_real_time_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 4000 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 1 /* Prioridade */
                , &handle_real_time_data ); /* Handle da tarefa, opcional  */
}