#include "BluetoothSerial.h"
#include "ELMduino.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial

uint8_t countError = 0;

ELM327 myELM327;
//teste2


int rpm = 0;
int fuelLevel = 0;
float bateryVoltage = 0;
float temperatura = 0;
float posAcel = 0;

float requestElmFloat(float function);
void errorElm();

void read_batery_voltage();
void read_RPM();
void read_fuel_level();
void check_data_elm();


void setup()
{
#if LED_BUILTIN
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
#endif

  DEBUG_PORT.begin(115200);
  //SerialBT.setPin("1234");
  ELM_PORT.begin("ArduHUD", true);
  
  while(!ELM_PORT.connected()){
    ELM_PORT.connect("OBDII");
    Serial.print(".");
    delay(200);
  }
  

  if (!myELM327.begin(ELM_PORT, true, 5000))
  {
    Serial.println("Couldn't connect to OBD scanner - Phase 2");
    while (1);
  }

  Serial.println("Connected to ELM327");
  delay(1000);
  myELM327.debugMode = false;
  
}


void loop()
{
  check_data_elm();
  //read_fuel_level();
  delay(500);
  read_RPM();
  delay(500);
  read_batery_voltage();
  delay(500);
  Serial.print("\nELM STATUS: "); Serial.print(myELM327.nb_rx_state);
  Serial.print("\nRPM: "); Serial.print(rpm);
  Serial.print("\nBatery Voltage: "); Serial.print(bateryVoltage);
  Serial.print("\nFuel Level: "); Serial.print(fuelLevel); Serial.print("%");
}

void check_data_elm(){
if(myELM327.nb_rx_state == ELM_NO_DATA)
{
  Serial.print("\nELM_NO_DATA Reseting\n");
  myELM327.begin(ELM_PORT, false, 5000);
}
}

void read_RPM()
{
  
 float tempRPM = myELM327.rpm();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    rpm = (uint32_t)tempRPM;
    
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    {myELM327.printError();}
}

void read_batery_voltage()
{
    
 float bateryTemp = myELM327.batteryVoltage();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
     bateryVoltage = bateryTemp;
    
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    {myELM327.printError();}
}

void read_fuel_level()
{
    
 float fuelTemp = myELM327.batteryVoltage();

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    fuelLevel = (uint32_t)fuelTemp;
   
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
    {myELM327.printError();}
}
