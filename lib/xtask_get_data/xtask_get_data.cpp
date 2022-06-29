#include "xtask_get_data.h"

TaskHandle_t handle_get_data;


void xTask_get_data( void *pvParameters )
{

    while (1)
    {
        elm.requestData();
        vTaskDelay(pdMS_TO_TICKS(100));
    }
    

}



void vTask_get_data_start()
{
    xTaskCreate( xTask_get_data /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_get_data_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 4000 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 1 /* Prioridade */
                , &handle_get_data ); /* Handle da tarefa, opcional  */
}