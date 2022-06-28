#ifndef INTERFACE_COM
#define INTERFACE_COM


#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "bluetooth_ble.h"


class Interface_com
{


    private:
        /* data */
    public:
        void requestData();
        void requestRealTimeData();

};











#endif