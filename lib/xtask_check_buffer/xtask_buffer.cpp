#include "xtask_buffer.h"

TaskHandle_t handle_check_buffer;



void xTask_check_buffer( void *pvParameters )
{



                                

    while (1)
    {
        elm.checkBuffer();

        vTaskDelay(pdMS_TO_TICKS(100));
    }
    

}



void vTask_check_buffer_start()
{
    xTaskCreate( xTask_check_buffer /* Funcao a qual esta implementado o que a tarefa deve fazer */
                , "xTask_check_buffer_DEBUG" /* Nome (para fins de debug, se necessário) */
                , 1500 /* Tamanho da stack (em words) reservada para essa tarefa */
                , NULL /* Parametros passados (nesse caso, não há) */
                , 5 /* Prioridade */
                , &handle_check_buffer ); /* Handle da tarefa, opcional  */
}