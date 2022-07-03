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
    double calcRpm = 0;
    double calcSpeed = 0;

    int lastTemp = 1;
    float lastVoltage = 1.0;
    int temp = 0;
    float voltage = 0.0;

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
            rpm = map(calcRpm, 0, 1.4, 315, 360);
            xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
            Serial.print("\nRPM BEFORE: ");
            Serial.print(calcRpm);
            Serial.print("\nRPM AFTER MAP: ");
            Serial.print(rpm);
            xSemaphoreGive(xSerial_semaphore);

        }else
        {
            rpm = map(calcRpm, 1.41, 8, 360, 220);
            xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
            Serial.print("\nRPM BEFORE2: ");
            Serial.print(calcRpm);
            Serial.print("\nRPM AFTER MAP2: ");
            Serial.print(rpm);
            xSemaphoreGive(xSerial_semaphore);
        }

        calcSpeed = car.get_speed();
        
        if (calcSpeed <= 39)
        {
            speed = map(calcSpeed, 0, 39, 315, 360);
            xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
            Serial.print("\nSPEED BEFORE: ");
            Serial.print(calcSpeed);
            Serial.print("\nSPEED AFTER MAP: ");
            Serial.print(speed);
            xSemaphoreGive(xSerial_semaphore);
        }else
        {
            speed = map(calcSpeed, 40, 240, 360, 220);
            xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
            Serial.print("\nSPEED BEFORE2: ");
            Serial.print(calcSpeed);
            Serial.print("\nSPEED AFTER MAP2: ");
            Serial.print(speed);
            xSemaphoreGive(xSerial_semaphore);
        }

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
        myNex.writeNum("z0.val", rpm);
        vTaskDelay(pdMS_TO_TICKS(50));
        xSemaphoreGive(xHardwareSerial_semaphore);
        

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
        myNex.writeNum("z1.val", speed);
        vTaskDelay(pdMS_TO_TICKS(50));
        xSemaphoreGive(xHardwareSerial_semaphore);

        temp = car.get_temp_01();

        if(temp != lastTemp)
        {

        lastTemp = temp;

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
        myNex.writeStr("t0.txt", (String) temp + "°C");
        vTaskDelay(pdMS_TO_TICKS(50)); 
        xSemaphoreGive(xHardwareSerial_semaphore);

        }

        voltage = car.get_batery_volt();

        if(voltage != lastVoltage)
        {
            lastVoltage = voltage;

        xSemaphoreTake(xHardwareSerial_semaphore, portMAX_DELAY);
        myNex.writeStr("t1.txt", (String) voltage + "V");
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