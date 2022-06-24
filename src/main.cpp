#include "main.h"

BtObd obdComm;


void setup()
{

obdComm.obd_init();
delay(500);

 
  
}


void loop()
{

obdComm.obd_send_pid(SERVICE_01, TEMP_ENGINE);
delay(500);
obdComm.obd_read_response();
delay(500);
obdComm.obd_send_pid(SERVICE_01, VOLTAGE_CONTROL_MODULE);
delay(500);

 
}

