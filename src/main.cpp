#include "BluetoothSerial.h"
#include "ELMduino.h"


BluetoothSerial SerialBT;
#define ELM_PORT   SerialBT
#define DEBUG_PORT Serial

uint8_t countError = 0;

ELM327 myELM327;
//teste2


float rpm = 0;
float temperatura = 0;
float posAcel = 0;

float requestElmFloat(float function);
void errorElm();

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
  

  if (!myELM327.begin(ELM_PORT, true, 3000))
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
  rpm = requestElmFloat(myELM327.rpm());
  temperatura = requestElmFloat(myELM327.engineCoolantTemp());
  posAcel = requestElmFloat(myELM327.throttle());

  Serial.print("\nRPM: " + (String)rpm);
  Serial.print("\nTEMPERATURA: " + (String)temperatura);
  Serial.print("\nACELERADOR: " + (String)posAcel);
  delay(100);
    
}

float requestElmFloat(float function){
  float temp = function;

if (myELM327.nb_rx_state == ELM_SUCCESS)
  {
    return temp;
  }
  else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
  {
    errorElm(); 
    return 99.9; //return
  }

}

void errorElm(){

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
      myELM327.begin(ELM_PORT, false, 3000);
      
    }

}
