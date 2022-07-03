#include "interface_com.h"

Car car;
//////////////////////////////////////////////////////////////////////////// REQUEST PID DATA
//////////////////////////////////////////////////////////////////////////// Envia comandos PID ao ELM

void ElmComm::_requestPidData(String service, String pid, String qtd_response)
{
    ble_send_pid(service, pid, qtd_response);
}





//////////////////////////////////////////////////////////////////////////// SEND COMMAND AT
//////////////////////////////////////////////////////////////////////////// Envia comandos AT ao ELM

void ElmComm::_sendCommandAt(String commandAt)
{
    ble_send_command_at(commandAt);
}



//////////////////////////////////////////////////////////////////////////// RESPONSE HANDLING
//////////////////////////////////////////////////////////////////////////// Faz o tratamento das respostas armazenadas no buffer de entrada

void ElmComm::_responseHandling(String response)
{
xSemaphoreTake(xTrat_semaphore, portMAX_DELAY);
    if (response.indexOf("AT") >= 0)
    {
        _reponseIsAt(response);
    } 

    if(response.indexOf("41") >= 0)
    {
        car.set_running(true);
        car.set_connecting(false);
        _responseIsPid(response);
    } 
    
    if(response.indexOf(AT_SEARCHING) >= 0)
    {
        car.set_connecting(true);
        car.set_running(false);
    } 
    if(response.indexOf(AT_UNABLE_TO_CONECT) >= 0)
    {
        car.set_running(false);
        car.set_connecting(false);

    }
xSemaphoreGive(xTrat_semaphore);
}






//////////////////////////////////////////////////////////////////////////// RESPONSE IS AT
//////////////////////////////////////////////////////////////////////////// Faz o tratamento das respostas para comandos AT
void ElmComm::_reponseIsAt(String response)
{
    if(response.indexOf(AT_RESET_ALL) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Resetado com sucesso!");
        xSemaphoreGive(xSerial_semaphore);
    } 
    else if(response.indexOf(AT_CABECALHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Cabecalho ligado! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_CABECALHO_OFF) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Cabecalho desligado! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_BATERY_VOLTAGE) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Tensao da bateria: ");
        int index = response.lastIndexOf(AT_BATERY_VOLTAGE);
        Serial.print(response.substring(index + 5, index + 11));
        car.set_batery_voltage(response.substring(index + 5, index + 10));
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_DEFAULT_RESTORE) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Restaurado para o padrao de fabrica!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_ECHO_OFF) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo OFF! ");
        xSemaphoreGive(xSerial_semaphore);
    }
    else if(response.indexOf(AT_ECHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo ON! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_CABECALHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo on!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_FAST_RESET) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Fast reset!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(response.indexOf(AT_PROTOCOLO_AUTO) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Protocolo automatico definido com sucesso!");
        xSemaphoreGive(xSerial_semaphore);

    }  
    else if(response.indexOf(AT_SEARCHING) >= 0)
    {

       car.set_running(false);
       car.set_connecting(true);

    }  
    else if(response.indexOf(AT_UNABLE_TO_CONECT) >= 0)
    {

       car.set_running(false);
       car.set_connecting(false);

    } 
    
}





//////////////////////////////////////////////////////////////////////////// RESPONSE IS PID
//////////////////////////////////////////////////////////////////////////// Faz o tratamento das respostas para comandos PID

void ElmComm::_responseIsPid(String response)
{
    
    int index = response.indexOf("41") + 3;
    int endIndex = index + 2;
    String bufferPid = response.substring(index, endIndex);
    String bufferResponse = response.substring(index + 2);

 // ESTAVA responsePID -> SUBSTITUIDO POR bufferResponse

    if(bufferPid.indexOf(SUPORTED_PIDS_01_20) >= 0)
    {
        int indexResp = bufferResponse.indexOf(SUPORTED_PIDS_01_20) + PID_LENGTH;
        car.set_suported_pids_01(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(STATUS_DTC) >= 0)
    {
        int indexResp = bufferResponse.indexOf(STATUS_DTC) + PID_LENGTH;
        car.set_status_dts(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(TEMP_ENGINE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(TEMP_ENGINE) + PID_LENGTH;
        car.set_temp_01(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(RPM_ENGINE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(RPM_ENGINE)  + PID_LENGTH;
        car.set_rpm(bufferResponse.substring(indexResp));

    }
    else if(bufferPid.indexOf(VEHICLE_SPEED) >= 0)
    {
         int indexResp = bufferResponse.indexOf(VEHICLE_SPEED) + PID_LENGTH;
        car.set_speed(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(POS_ACEL) >= 0)
    {
         int indexResp = bufferResponse.indexOf(POS_ACEL) + PID_LENGTH;
        car.set_acel_pos(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(SUPORTED_PIDS_21_40) >= 0)
    {
        int indexResp = bufferResponse.indexOf(SUPORTED_PIDS_21_40) + PID_LENGTH;
        car.set_suported_pids_21(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(SUPORTED_PIDS_41_60) >= 0)
    {
        int indexResp = bufferResponse.indexOf(SUPORTED_PIDS_41_60) + PID_LENGTH;
        car.set_suported_pids_41(bufferResponse.substring(indexResp));
    }

     else if(bufferPid.indexOf(FUEL_TYPE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(FUEL_TYPE) + PID_LENGTH;
        car.set_fuel_type(bufferResponse.substring(indexResp));
    }
}





//////////////////////////////////////////////////////////////////////////// Check comm
//////////////////////////////////////////////////////////////////////////// Verifica o status da comunicação

void ElmComm::_checkComm()
{       
    if(car.is_connecting())
   {
    xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
    Serial.print("\nCONECTANDO...");
    xSemaphoreGive(xSerial_semaphore);
    car.set_connecting(true);
     
   }else if(!car.is_running())
   {
    xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
    Serial.print("\nVEICULO NAO ENCONTRADO!");
    xSemaphoreGive(xSerial_semaphore);
    car.set_running(false);
     
   }else if(car.is_running())
   {
    
   }

   if(car.is_connecting() || !car.is_running())
   {
    ble_send_pid(SERVICE_01, SUPORTED_PIDS_01_20, "1"); // TESTE DE COMUNICAÇÃO
    vTaskDelay(pdMS_TO_TICKS(1000));
    //ble_send_command_at(AT_BATERY_VOLTAGE);
    //vTaskDelay(pdMS_TO_TICKS(50));

   }
    
}




//////////////////////////////////////////////////////////////////////////// Check comm
//////////////////////////////////////////////////////////////////////////// Verifica o status da comunicação
void ElmComm::initialCommandsAt()
{
    Serial.println("\nBLE Conectado!");
    ble_send_command_at(AT_DEFAULT_RESTORE);
    delay(200);
    ble_send_command_at(AT_CABECALHO_OFF);
    delay(100);
    ble_send_command_at(AT_PROTOCOLO_AUTO);
    delay(100);
    ble_send_command_at(AT_BATERY_VOLTAGE);
    delay(100);
    ble_send_command_at(AT_ECHO_ON);
    delay(100);
    ble_send_command_at(AT_IB_10); /// VERIFICAR
    delay(100);
}





//////////////////////////////////////////////////////////////////////////// Request Data
//////////////////////////////////////////////////////////////////////////// Requisita os dados do veiculo
void ElmComm::requestData()
{
    ble_send_pid(SERVICE_01, TEMP_ENGINE, "1");
   // ble_send_pid(SERVICE_01, FUEL_TYPE, "1");
    ble_send_command_at(AT_BATERY_VOLTAGE);
}





//////////////////////////////////////////////////////////////////////////// Request Real Time Data
//////////////////////////////////////////////////////////////////////////// Requisita os dados RT do veiculo
void ElmComm::requestRealTimeData()
{
    ble_send_pid(SERVICE_01, RPM_ENGINE, "1");
    vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
    ble_send_pid(SERVICE_01, VEHICLE_SPEED, "1");
    vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
   // ble_send_pid(SERVICE_01, POS_ACEL, "1");
   // vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
}




//////////////////////////////////////////////////////////////////////////// Loop
//////////////////////////////////////////////////////////////////////////// Faz verificação do estado da comunicação e reconecta se necessario
void ElmComm::elm_loop()
{
if (doConnect == true) {
    if (connectToServer()) {

        this->initialCommandsAt();
        vTaskDelay(pdMS_TO_TICKS(2000));
    } else {
      Serial.println("\nBle não conectado!");
    }
    doConnect = false;
  }

  if (connected) {
    
    this->_checkComm();
   
   

  }else if(doScan)
  {
    Serial.print("\nPerdeu a comunicação!");
    ble_reconnect();
  }
  
}





//////////////////////////////////////////////////////////////////////////// ELM SETUP
//////////////////////////////////////////////////////////////////////////// Faz as configurações iniciais do Bluetooth
void ElmComm::elm_setup()
{
    ble_client_setup();
}









//////////////////////////////////////////////////////////////////////////// ELM CHECK BUFFER
//////////////////////////////////////////////////////////////////////////// Verifica se há alguma coisa no buffer de entrada
void ElmComm::checkBuffer()
{
    std::string *pStr = NULL;
    //String response = "null";
    xQueueReceive(xQueue_bufferEntrada, (void *) &pStr, 100);
    
    if(pStr != NULL)
    {
        this->_responseHandling(pStr->c_str());
    }
    

    delete pStr;
}







