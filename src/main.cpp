#include "BluetoothSerial.h"
#include "ELMduino.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial

uint8_t countError = 0;

ELM327 myELM327;
//teste2

uint32_t rpm = 0;


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
  float tempRPM = myELM327.rpm();
  delay(200);
  float engineTemp = myELM327.engineCoolantTemp();
  delay(200);
  float posAcel = myELM327.throttle();
  delay(200);

  if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    Serial.print("\nThrottle: "); Serial.print(posAcel);
    Serial.print("\nEngine Temp: "); Serial.print(engineTemp);
    rpm = (uint32_t)tempRPM;
    Serial.print("\nRPM: "); Serial.println(rpm);
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
  {
    myELM327.printError();
    Serial.print("\n STATUS BLUETOOTH: ");
    Serial.print(ELM_PORT.connected());
    Serial.print("\n STATUS CONEXAO ELM327: ");
    Serial.print(myELM327.connected);
    Serial.print("\n");
    
    if(countError <= 5){
      countError++;
    Serial.print("\nCount Errors: ");
    Serial.print(countError);

    }else{
      Serial.print("\nTentando reconexao com a ECU");
      countError = 0;
      myELM327.begin(ELM_PORT, false, 5000);
      
    }
    
    
    
  }
    
}