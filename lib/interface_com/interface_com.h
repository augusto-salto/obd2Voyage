#ifndef INTERFACE_COM
#define INTERFACE_COM


#include "Arduino.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "bluetooth_ble.h"
#include "pid.h"
#include "car.h"


extern boolean doConnect;
extern boolean connected;
extern boolean doScan;

extern xQueueHandle xQueue_bufferEntrada;


class ElmComm
{

    private:
        void _requestPidData(String service, String pid, String qtd_response);  // Envia comandos PID ao ELM
        void _sendCommandAt(String commandAt);                                  // Envia comandos AT ao ELM
        void _responseHandling(String response);                                // Faz o tratamento das respostas armazenadas no buffer de entrada                                             
        void _reponseIsAt(String response);                                     // Recebe e trata todas as respostas para comandos AT
        void _responseIsPid(String response);                                   // Recebe e trata todas as respostas para comandos PID
        void _checkComm();                                                      // Checa a comunicação
        


    public:
        void initialCommandsAt();                                               // Envia os comandos AT iniciais
        void requestData();                                                     // Requisita os dados do ELM
        void requestRealTimeData();                                             // Requisita os dados RT do ELM
        void bufferRead();                                                      // Lê os dados armazenados no buffer de entrada                    
        void elm_loop();                                                        // Faz verificações do estado da comunicação, e reconecta se necessario
        void elm_setup();                                                       // Faz as configurações iniciais da comunicação
        void checkBuffer();                                                    // Verifica se há alguma informação no buffer de entrada

};











#endif