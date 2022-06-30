#include "xtask_display.h"


void xTask_display( void *pvParameters )
{

    while (1)
    {
       
        vTaskDelay(pdMS_TO_TICKS(10));
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