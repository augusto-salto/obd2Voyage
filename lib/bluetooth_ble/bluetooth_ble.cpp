#include "bluetooth_ble.h"

//Car car;

static BLEUUID serviceUUID("0000fff0-0000-1000-8000-00805f9b34fb");
static BLEUUID charRxUUID("0000fff1-0000-1000-8000-00805f9b34fb");
static BLEUUID charTxUUID("0000fff2-0000-1000-8000-00805f9b34fb");

 boolean doConnect = false;
 boolean connected = false;
 boolean doScan = false;
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
    
    //selectResponse((char*)pData);
    String bufferEntrada = (char*)pData;
    std::string *pStr = new std::string((char*)pData);

   // Serial.print("\nSIZE OF: ");
//    Serial.print(sizeof((char*)pData));

    xQueueSend(xQueue_bufferEntrada, (void *)&pStr, portMAX_DELAY);
   
    //AQUI VAI ARMAZENAR NO BUFFER
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
        if(countBle >= 3)
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


void ble_reconnect()
{
    BLEDevice::getScan()->start(0);  
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
        pRemoteCharacteristicTx->writeValue(buff_send.c_str(), buff_send.length());
    }

}

















