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

// ******************************************************************** Callback de recebimento de notificação
static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {
    Serial.print("\nRecebido: ");
    Serial.print((char*)pData);
    selectResponse((char*)pData);
    
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

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks


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
      Serial.println("\nBLE Conectado!");
      ble_send_command_at(AT_FAST_RESET);
      delay(100);
      ble_send_command_at(AT_CABECALHO_OFF);
      delay(100);
      ble_send_command_at(AT_ECHO_ON);
      delay(100);
      ble_send_command_at(AT_PROTOCOLO_AUTO);
      delay(100);
      ble_send_command_at(AT_BATERY_VOLTAGE);
      delay(100);
      
    } else {
      Serial.println("\nBle não conectado!");
    }
    doConnect = false;
  }

  // If we are connected to a peer BLE Server, update the characteristic each time we are reached
  // with the current time since boot.
  if (connected) {
    //String newValue = "AT SP 00";
    //Serial.println("\nEnviando: " + newValue);
    
    // Set the characteristic's value to be the array of bytes that is actually a string.
    //pRemoteCharacteristicTx->writeValue(newValue.c_str(), newValue.length());
    //Serial.print("Perdeu a comunicação!");

#if DEBUG == 1
    if(Serial.available()){
        String envia = Serial.readString();
        envia += '\r';
        Serial.print("\nEnviando: " + envia);
        pRemoteCharacteristicTx->writeValue(envia.c_str(), envia.length());
    }
#endif

  }else if(doScan){
    Serial.print("\nPerdeu a comunicação!");
    BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
  }
  
  delay(500); // Delay a second between loops.
} 

void ble_send_pid(String service, String pid)
{
    if(connected)
    {
        String buff_send = service + pid;
        buff_send += '\r';
        Serial.print("\nEnviando: " + buff_send);
        pRemoteCharacteristicTx->writeValue(buff_send.c_str(), buff_send.length());
    }
    
}


void ble_send_command_at(String command)
{   
    if(connected)
    {
        String buff_send = command;
        buff_send += '\r';
        Serial.print("\nEnviando: " + buff_send);
        pRemoteCharacteristicTx->writeValue(buff_send.c_str(), buff_send.length());
    }

}

void selectResponse(String response)
{
    // BUFFER
    /*response.trim();
    response.replace(" ", "");
    char buffer[response.length() + 1];
    response.toCharArray(buffer, response.length() + 1);

    Serial.print("\n >>>>>>> BUFFER DE RESPOSTA: ");
    for (int i = 0; i<response.length(); i++)
    {
        Serial.print("\nBuffer[");
        Serial.print(i);
        Serial.print("] :");
        Serial.print(buffer[i]);
    }
    Serial.print("\n >>>>>>>> FIM DO BUFFER");*/

    if (response.indexOf("AT") >= 0)
    {
        onReceiveCommandAT(response);
    } 
    else if(response.indexOf("41") >= 0)
    {
        onReceivedPid(response);
    } 
    else 
    {
        Serial.print("\nComando invalido!");
    }

}

void onReceiveCommandAT(String responseAt)
{
    if(responseAt.indexOf(AT_RESET_ALL) >= 0)
    {
        Serial.print("\nELM327: Resetado com sucesso!");
    } 
    else if(responseAt.indexOf(AT_CABECALHO_ON) >= 0)
    {
        Serial.print("\nELM327: Cabecalho ligado! ");
    }
     else if(responseAt.indexOf(AT_CABECALHO_OFF) >= 0)
    {
        Serial.print("\nELM327: Cabecalho desligado! ");
    }
     else if(responseAt.indexOf(AT_BATERY_VOLTAGE) >= 0)
    {
        Serial.print("\nELM327: Tensao da bateria: ");
        int index = responseAt.lastIndexOf(AT_BATERY_VOLTAGE);
        Serial.print(responseAt.substring(index + 5));
    }
     else if(responseAt.indexOf(AT_DEFAULT_RESTORE) >= 0)
    {
        Serial.print("\nELM327: Restaurado para o padrao de fabrica!");
    }
     else if(responseAt.indexOf(AT_ECHO_OFF) >= 0)
    {
        Serial.print("\nELM327: Echo OFF! ");
    }
    else if(responseAt.indexOf(AT_ECHO_ON) >= 0)
    {
        Serial.print("\nELM327: Echo ON! ");
    }
     else if(responseAt.indexOf(AT_CABECALHO_ON) >= 0)
    {
        Serial.print("\nELM327: Echo on!");
    }
     else if(responseAt.indexOf(AT_FAST_RESET) >= 0)
    {
        Serial.print("\nELM327: Fast reset!");
    }
     else if(responseAt.indexOf(AT_PROTOCOLO_AUTO) >= 0)
    {
        Serial.print("\nELM327: Protocolo automatico definido com sucesso!");
    } else if(responseAt.indexOf(AT_BATERY_VOLTAGE) >= 0)
    {
        int indexResp = responseAt.indexOf(AT_BATERY_VOLTAGE + 5);
        car.set_batery_voltage(responseAt.substring(indexResp));
    }

}

void onReceivedPid(String responsePID)
{
    // Especificar qual a resposta... ex: 41 00 
    int index = responsePID.indexOf("41") + 3;  // 3 OU 4 TODO!
    int endIndex = index + 2;
    String bufferPid = responsePID.substring(index, endIndex);

    if(bufferPid.indexOf(SUPORTED_PIDS_01_20) >= 0)
    {
        int indexResp = responsePID.indexOf(SUPORTED_PIDS_01_20 + PID_LENGTH);
        car.set_suported_pids_01(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(STATUS_DTC) >= 0)
    {
        int indexResp = responsePID.indexOf(STATUS_DTC + PID_LENGTH);
        car.set_status_dts(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(TEMP_ENGINE) >= 0)
    {
        int indexResp = responsePID.indexOf(TEMP_ENGINE + PID_LENGTH);
        car.set_temp_01(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(RPM_ENGINE) >= 0)
    {
        int indexResp = responsePID.indexOf(RPM_ENGINE + PID_LENGTH);
        car.set_rpm(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(VEHICLE_SPEED) >= 0)
    {
         int indexResp = responsePID.indexOf(VEHICLE_SPEED + PID_LENGTH);
        car.set_speed(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(POS_ACEL) >= 0)
    {
         int indexResp = responsePID.indexOf(POS_ACEL + PID_LENGTH);
        car.set_acel_pos(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(SUPORTED_PIDS_21_40) >= 0)
    {
        int indexResp = responsePID.indexOf(SUPORTED_PIDS_21_40 + PID_LENGTH);
        car.set_suported_pids_21(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(FUEL_TANK_LEVEL_INPUT) >= 0)
    {
        int indexResp = responsePID.indexOf(FUEL_TANK_LEVEL_INPUT + PID_LENGTH);
        car.set_fuel_level(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(SUPORTED_PIDS_41_60) >= 0)
    {
        int indexResp = responsePID.indexOf(SUPORTED_PIDS_41_60 + PID_LENGTH);
        car.set_suported_pids_41(responsePID.substring(indexResp));
    }
    else if(bufferPid.indexOf(VOLTAGE_CONTROL_MODULE) >= 0)
    {
        int indexResp = responsePID.indexOf(VOLTAGE_CONTROL_MODULE + PID_LENGTH);
        car.set_voltage(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(TEMP_AMBIENTE) >= 0)
    {
        int indexResp = responsePID.indexOf(TEMP_AMBIENTE + PID_LENGTH);
        car.set_ambient_temp(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(FUEL_TYPE) >= 0)
    {
        int indexResp = responsePID.indexOf(FUEL_TYPE + PID_LENGTH);
        car.set_fuel_type(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(FUEL_RATE) >= 0)
    {
        int indexResp = responsePID.indexOf(FUEL_RATE + PID_LENGTH);
        car.set_fuel_rate(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(SUPORTED_PIDS_61_80) >= 0)
    {
        int indexResp = responsePID.indexOf(SUPORTED_PIDS_61_80 + PID_LENGTH);
        car.set_suported_pids_61(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(TEMP_ENGINE_COOLANT) >= 0)
    {
        int indexResp = responsePID.indexOf(TEMP_ENGINE_COOLANT + PID_LENGTH);
        car.set_coolant_temp(responsePID.substring(indexResp));
    }
     else if(bufferPid.indexOf(ODOMETER) >= 0)
    {
        int indexResp = responsePID.indexOf(ODOMETER + PID_LENGTH);
        car.set_odometer(responsePID.substring(indexResp));
    }
    
    


}



