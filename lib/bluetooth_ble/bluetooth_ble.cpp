#include "bluetooth_ble.h"

Car car;

static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");
static BLEUUID charRxUUID("0000fff1-0000-1000-8000-00805f9b34fb");
static BLEUUID charTxUUID("0000fff2-0000-1000-8000-00805f9b34fb");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristicRx;
static BLERemoteCharacteristic* pRemoteCharacteristicTx;
static BLEAdvertisedDevice* myDevice;

uint8_t countBle = 0;
// ******************************************************************** Callback de recebimento de notificação
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
   // xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
   // Serial.print("\nRecebido: ");
   // Serial.print((char*)pData);
   // xSemaphoreGive(xSerial_semaphore);
    xSemaphoreTake(xBle_semaphore, portMAX_DELAY ); 
    selectResponse((char*)pData);
    xSemaphoreGive(xBle_semaphore);
    
}

// ******************************************************************** Callback de conexao / desconexao
class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("onDisconnect");
  }
};

// ******************************************************************** Conexao
bool connectToServer() {
    Serial.print("Conectando a: ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    BLEClient*  pClient  = BLEDevice::createClient();
    pClient->setClientCallbacks(new MyClientCallback());
    pClient->connect(myDevice);  
    Serial.println("- Connected to server");
    pClient->setMTU(517); 
  
    // ******************************************************************** Config Serviço 
    BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(serviceUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println("- Found our service");


    // ******************************************************************** Config Caracteristicas
    pRemoteCharacteristicRx = pRemoteService->getCharacteristic(charRxUUID);
     pRemoteCharacteristicTx = pRemoteService->getCharacteristic(charTxUUID);
    if (pRemoteCharacteristicRx == nullptr) {
      Serial.print("Failed to find our characteristic UUID: ");
      Serial.println(charRxUUID.toString().c_str());
      pClient->disconnect();
      return false;
    }
    Serial.println("- Found our characteristic");

  
    if(pRemoteCharacteristicRx->canRead()) {
      std::string value = pRemoteCharacteristicRx->readValue();
      Serial.print("The characteristic value was: ");
      Serial.println(value.c_str());
    }

    if(pRemoteCharacteristicRx->canNotify())
      pRemoteCharacteristicRx->registerForNotify(notifyCallback);

    connected = true;
    return true;
}

    // ******************************************************************** BLE Scan Device

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("Dispositivos BLE encontrados: ");
    Serial.println(advertisedDevice.toString().c_str());

    // Conecta ao dispositivo que tiver o serviço configurado
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    }  else
    {
        countBle++;
        if(countBle >= 1)
        {
            Serial.print("Não encontrado! Reiniciando!");
            vTaskDelay(pdMS_TO_TICKS(500));
            ESP.restart();
        }
    }
  } 
}; 


void ble_client_setup() {
  
  Serial.println("Iniciando BLE");
  BLEDevice::init("");
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);
  
} 


void ble_client_loop() {

  if (doConnect == true) {
    if (connectToServer()) {
     
        ble_at_config();
        vTaskDelay(pdMS_TO_TICKS(1000));
    } else {
      Serial.println("\nBle não conectado!");
    }
    doConnect = false;
  }

  if (connected) {
    ble_check_comm();

#if DEBUG == 1
    if(Serial.available()){
        String envia = Serial.readString();
        envia += '\r';
        Serial.print("\nEnviando: " + envia);
        pRemoteCharacteristicTx->writeValue(envia.c_str(), envia.length());
    }
#endif

  }else if(doScan)
  {
    Serial.print("\nPerdeu a comunicação!");
    BLEDevice::getScan()->start(0);  
  }
  
  delay(500); 
} 

void ble_send_pid(String service, String pid, String qtd_response)
{
    if(connected)
    {
        String buff_send = service + pid + qtd_response;
        buff_send += '\r';
        pRemoteCharacteristicTx->writeValue(buff_send.c_str(), buff_send.length());
    }
    
}


void ble_send_command_at(String command)
{   
    if(connected)
    {
        String buff_send = command;
        buff_send += '\r';
       // Serial.print("\nEnviando: " + buff_send);
        pRemoteCharacteristicTx->writeValue(buff_send.c_str(), buff_send.length());
    }

}

void selectResponse(String response)
{
    int flag = false;
    if (response.indexOf("AT") >= 0 && !flag)
    {
        
        onReceiveCommandAT(response);
        //flag = true;
    } 

    if(response.indexOf("41") >= 0&& flag == false)
    {
        car.set_running(true);
        car.set_connecting(false);
        onReceivedPid(response);
        //flag = true;
    } 
    
    if(response.indexOf(AT_SEARCHING) >= 0 && flag == false)
    {
        car.set_connecting(true);
        car.set_running(false);
        //flag = true;
    } 
    if(response.indexOf(AT_UNABLE_TO_CONECT) >= 0 && flag == false)
    {
        car.set_running(false);
        car.set_connecting(false);
        //flag = true;
    }
   /* if(flag == false)
    {
        Serial.print(" IF FLAG");
        //xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
       // Serial.print("\nResposta nao tratada: ");
       // Serial.print(response);
        //xSemaphoreGive(xSerial_semaphore);
        //flag = true;
    }*/


}

void onReceiveCommandAT(String responseAt)
{   
    
    if(responseAt.indexOf(AT_RESET_ALL) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Resetado com sucesso!");
        xSemaphoreGive(xSerial_semaphore);
    } 
    else if(responseAt.indexOf(AT_CABECALHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Cabecalho ligado! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_CABECALHO_OFF) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Cabecalho desligado! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_BATERY_VOLTAGE) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Tensao da bateria: ");
        int index = responseAt.lastIndexOf(AT_BATERY_VOLTAGE);
        Serial.print(responseAt.substring(index + 5, index + 11));
        car.set_batery_voltage(responseAt.substring(index + 5, index + 10));
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_DEFAULT_RESTORE) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Restaurado para o padrao de fabrica!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_ECHO_OFF) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo OFF! ");
        xSemaphoreGive(xSerial_semaphore);
    }
    else if(responseAt.indexOf(AT_ECHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo ON! ");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_CABECALHO_ON) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Echo on!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_FAST_RESET) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Fast reset!");
        xSemaphoreGive(xSerial_semaphore);
    }
     else if(responseAt.indexOf(AT_PROTOCOLO_AUTO) >= 0)
    {
        xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
        Serial.print("\nELM327: Protocolo automatico definido com sucesso!");
        xSemaphoreGive(xSerial_semaphore);

    } else if(responseAt.indexOf(AT_BATERY_VOLTAGE) >= 0)
    {
        //int indexResp = responseAt.indexOf(AT_BATERY_VOLTAGE + 5);
        //car.set_batery_voltage(responseAt.substring(indexResp));
    }
    
}

void onReceivedPid(String responsePID)
{

    int index = responsePID.indexOf("41") + 3;
    int endIndex = index + 2;
    String bufferPid = responsePID.substring(index, endIndex);
    String bufferResponse = responsePID.substring(index + 2);

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
    else if(bufferPid.indexOf(FUEL_TANK_LEVEL_INPUT) >= 0)
    {
        int indexResp = bufferResponse.indexOf(FUEL_TANK_LEVEL_INPUT) + PID_LENGTH;
        car.set_fuel_level(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(SUPORTED_PIDS_41_60) >= 0)
    {
        int indexResp = bufferResponse.indexOf(SUPORTED_PIDS_41_60) + PID_LENGTH;
        car.set_suported_pids_41(bufferResponse.substring(indexResp));
    }
    else if(bufferPid.indexOf(VOLTAGE_CONTROL_MODULE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(VOLTAGE_CONTROL_MODULE) + PID_LENGTH;
        car.set_voltage(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(TEMP_AMBIENTE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(TEMP_AMBIENTE) + PID_LENGTH;
        car.set_ambient_temp(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(FUEL_TYPE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(FUEL_TYPE) + PID_LENGTH;
        car.set_fuel_type(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(FUEL_RATE) >= 0)
    {
        int indexResp = bufferResponse.indexOf(FUEL_RATE) + PID_LENGTH;
        car.set_fuel_rate(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(SUPORTED_PIDS_61_80) >= 0)
    {
        int indexResp = bufferResponse.indexOf(SUPORTED_PIDS_61_80) + PID_LENGTH;
        car.set_suported_pids_61(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(TEMP_ENGINE_COOLANT) >= 0)
    {
        int indexResp = bufferResponse.indexOf(TEMP_ENGINE_COOLANT) + PID_LENGTH;
        car.set_coolant_temp(bufferResponse.substring(indexResp));
    }
     else if(bufferPid.indexOf(ODOMETER) >= 0)
    {
        int indexResp = bufferResponse.indexOf(ODOMETER) + PID_LENGTH;
        car.set_odometer(bufferResponse.substring(indexResp));
    }
    
    


}

void ble_check_comm()
{
    
    if(car.is_connecting())
   {
    xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
    Serial.print("\nCONECTANDO...");
    xSemaphoreGive(xSerial_semaphore);
     
   }else if(!car.is_running())
   {
    xSemaphoreTake(xSerial_semaphore, portMAX_DELAY);
    Serial.print("\nVEICULO NAO ENCONTRADO!");
    xSemaphoreGive(xSerial_semaphore);
     
   }else if(car.is_running())
   {
       // ble_send_pid(SERVICE_01, SUPORTED_PIDS_01_20, "1");
     // Serial.print("VEÍCULO CONECTADO!"); 
   }

   if(car.is_connecting() || !car.is_running())
   {
    ble_send_pid(SERVICE_01, SUPORTED_PIDS_01_20, "1"); // TESTE DE COMUNICAÇÃO
    vTaskDelay(pdMS_TO_TICKS(500));
    ble_send_command_at(AT_BATERY_VOLTAGE);
    vTaskDelay(pdMS_TO_TICKS(500));

   }
    
}



void ble_at_config()
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


void ble_get_real_time_data()
{
    
    ble_send_pid(SERVICE_01, RPM_ENGINE, "1");
    vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
    ble_send_pid(SERVICE_01, VEHICLE_SPEED, "1");
    vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
    ble_send_pid(SERVICE_01, POS_ACEL, "1");
    vTaskDelay(pdMS_TO_TICKS(REAL_TIME_DELAY));
    
}

void ble_get_data()
{
    ble_send_pid(SERVICE_01, TEMP_ENGINE, "1");
    ble_send_pid(SERVICE_01, FUEL_TANK_LEVEL_INPUT, "1");
    ble_send_pid(SERVICE_01, VOLTAGE_CONTROL_MODULE, "1");
    ble_send_pid(SERVICE_01, TEMP_AMBIENTE, "1");
    ble_send_pid(SERVICE_01, FUEL_TYPE, "1");
    ble_send_pid(SERVICE_01, FUEL_RATE, "1");
    ble_send_pid(SERVICE_01, ODOMETER, "1");
    ble_send_command_at(AT_BATERY_VOLTAGE);
}