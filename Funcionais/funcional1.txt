#include "BluetoothSerial.h"
#include "ELMduino.h"
BluetoothSerial SerialBT;
ELM327 myELM327;

byte inData;
char inChar;
String BuildINString = "";
String request = "";
void setup()
{


  Serial.begin(115200);
  Serial.println("Iniciando o Bluetooth...");
  SerialBT.end();
  Serial.print(SerialBT.begin("esp", true));
  SerialBT.clearWriteError();
  SerialBT.flush();
  //SerialBT.setPin("1234");
  Serial.println("Conectando...");
  SerialBT.connect("OBDII");
  
  

  while(!SerialBT.connected()){
    SerialBT.connect("OBDII");
    Serial.print(".");
  }

  
//myELM327.begin(Serial, false, 2000);

Serial.println("\n\nConnected to ELM327");
  
}


void loop()
{
BuildINString = ""; 
BuildINString.trim();
BuildINString.clear();

request = "";

if(Serial.available())
{
  request = Serial.readString();
  Serial.print("\nECHO: "  + request);
  SerialBT.println(request);
  Serial.print("\nEnviando request...");
}

//SerialBT.println("AT SP 00"); // -----> funciona!


while(SerialBT.available() > 0)
{
  //Serial.print("\nResposta:");
  //Serial.print(SerialBT.read());
  
  inData=0;
  inChar=0;
  inData = SerialBT.read();
  inChar=char(inData);
  BuildINString = BuildINString + inChar; 
}

if( !BuildINString.isEmpty() && !BuildINString.equals(" ") && !BuildINString.equals(""))
{
Serial.print("\nResposta: ");
Serial.print(BuildINString);
}



delay(50);
 
}

