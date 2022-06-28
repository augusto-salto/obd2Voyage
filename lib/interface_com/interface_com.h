#ifndef INTERFACE_COM
#define INTERFACE_COM


#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "bluetooth_ble.h"
#include "pid.h"
#include "car.h"

extern Car car;




class ElmComm
{

    private:
        void _requestPidData(String service, String pid, String qtd_response);  // Envia comandos PID ao ELM
        void _sendCommandAt(String commandAt);                                  // Envia comandos AT ao ELM
        void _responseHandling();                                               // Faz o tratamento das respostas armazenadas no buffer de entrada                                             
        void _reponseIsAt(String response);                                     // Recebe e trata todas as respostas para comandos AT
        void _responseIsPid(String response);                                   // Recebe e trata todas as respostas para comandos PID



    public:
        void initialCommandsAt();                                               // Envia os comandos AT iniciais
        void requestData();                                                     // Requisita os dados do ELM
        void requestRealTimeData();                                             // Requisita os dados RT do ELM
        void bufferRead();                                                      // Lê os dados armazenados no buffer de entrada                    
        void checkComm();                                                       // Checa a comunicação
};











#endif