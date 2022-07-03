#include "xtask_display.h"

HardwareSerial NextionSerial(2);

void xTask_display( void *pvParameters )
{
    EasyNex myNex(NextionSerial);
    myNex.begin(9600);
    myNex.writeStr("page page1");
    vTaskDelay(pdMS_TO_TICKS(2000));
    myNex.writeStr("page page2");

    int rpm = 0;
    int speed = 0;
    float calcRpm = 0;
    float calcSpeed = 0;

    int lastrpm = 1;
    int lastspeed = 1;


    int lastTemp = 1;
    float lastVoltage = 1.0;
    int temp = 0;
    float voltage = 0.0;

    String temporary = "";

    while (car.is_connecting() || !car.is_running())
    {
        vTaskDelay(10);
    }

    myNex.writeStr("page page0");
    vTaskDelay(500);


    while (1)
    {
        calcRpm = car.get_rpm();
        calcRpm = calcRpm/1000;
 


        if (calcRpm <= 1.4)
        {
            rpm = mapfloat(calcRpm, 0, 1.4, 315, 360);
         

        }else
        {
            rpm = mapfloat(calcRpm, 1.4, 9, 0, 230);
        
        }

        calcSpeed = car.get_speed();
        
        if (calcSpeed <= 39)
        {
            speed = map(calcSpeed, 0, 39, 315, 360);
            
           
        }else
        {
            speed = map(calcSpeed, 40, 240, 0, 220);
            
        }

        if(rpm != lastrpm)
        {
            lastrpm = rpm;
            xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
            myNex.writeNum("z0.val", rpm);
            vTaskDelay(pdMS_TO_TICKS(50));
            xSemaphoreGive(xHardwareSerial_semaphore);

        }

        
        if (speed != lastspeed)
        {

        lastspeed = speed;
        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
        myNex.writeNum("z1.val", speed);
        vTaskDelay(pdMS_TO_TICKS(50));
        xSemaphoreGive(xHardwareSerial_semaphore);

        }


       
        

        temp = car.get_temp_01();

        if(temp != lastTemp)
        {

        lastTemp = temp;

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);

        temporary = String(temp);
        temporary += "C";

        myNex.writeStr("t0.txt", temporary);
        vTaskDelay(pdMS_TO_TICKS(50)); 
        xSemaphoreGive(xHardwareSerial_semaphore);

        }

        voltage = car.get_batery_volt();

        if(voltage != lastVoltage)
        {
            lastVoltage = voltage;

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);

        temporary  = String(voltage, 1);
        temporary += "V";

        myNex.writeStr("t1.txt", temporary);
        vTaskDelay(pdMS_TO_TICKS(50));   
        xSemaphoreGive(xHardwareSerial_semaphore);  


        }
        

        

        
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

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void rpmLogic(int last, int current)  /// LATER
{
    signed int diference = last - current;

    if (diference > 0)
    {
        for (int i = 0; i <= diference; i++)
        {
            last++;
        }
    }    
}