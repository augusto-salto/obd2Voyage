#include "BluetoothSerial.h"
#include "ELMduino.h"
BluetoothSerial SerialBT;

float rpm = 0;
float fuelLevel = 0;
float bateryVoltage = 0;
float temperatura = 0;
float posAcel = 0;
byte message[] = {0x01, 0x00 };
byte message2[] = {0x01, 0x05 };
char buff[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
String teste;

int receive = 0;

void setup()
{


  Serial.begin(115200);
  SerialBT.begin("esp", true);

  SerialBT.connect("OBDII");
  Serial.println("Conectando...");

  while(!SerialBT.connected()){
    SerialBT.connect("OBDII");
    Serial.print(".");
  }
  

  Serial.println("Connected to ELM327");
  delay(1000);
SerialBT.print("4849 4848");
  delay(50);
  Serial.print("\nResposta (SETUP): ");
  Serial.print(SerialBT.read());
}


void loop()
{

SerialBT.print("4849 4853");
delay(50);
Serial.print("\nResposta (LOOP READ): ");
Serial.print(SerialBT.read());

delay(200);

SerialBT.print("4849 4853");
delay(50);
Serial.print("\nResposta (LOOP READ String): ");
Serial.print(SerialBT.readString());






delay(500); 
 
}

