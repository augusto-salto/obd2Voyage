#ifndef BLUETOOTH_BLE_H
#define BLUETOOTH_BLE_H


#include <BLEDevice.h>
#include <Arduino.h>
#include "car.h"


#define bleServerName "OBDBLE"
#define DEBUG 0

#define REAL_TIME_DELAY 80

#define SERVICE_UUID                                     "0000fff0-0000-1000-8000-00805f9b34fb" 

#define CHARACTERISTIC_1                                 "0000fff1-0000-1000-8000-00805f9b34fb"
#define DESCRIPTOR_1                                     "00002902-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_2                                 "0000fff2-0000-1000-8000-00805f9b34fb" 

extern SemaphoreHandle_t xBle_semaphore;

void ble_client_loop();
void ble_client_setup();

//void selectResponse(String response);
void ble_send_command_at(String command);
void ble_send_pid(String service, String pid, String qtd_response = "");
//void onReceiveCommandAT(String responseAt);
//void onReceivedPid(String responsePID);

//void ble_at_config();
//void ble_get_real_time_data();
//void ble_get_data();
void ble_check_comm();
void ble_reconnect();
bool connectToServer();














#endif