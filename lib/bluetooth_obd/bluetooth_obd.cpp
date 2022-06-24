#include "bluetooth_obd.h"

BluetoothSerial SerialBT;
byte inData;
char inChar;
String BuildINString = "";
String request = "";







////////////////////////////////////////////////////////////// CONSTRUTOR
BtObd::BtObd()
{

    Serial.begin(115200);
    Serial.println("Iniciando o Bluetooth...");
    Serial.print(SerialBT.begin("esp", true));
    Serial.println("Conectando...");
    SerialBT.connect("OBDII");

    while(!SerialBT.connected(10000))
      {
        SerialBT.connect();
        Serial.print(".");
      }

    Serial.println("\n\nConnected to ELM327");
}









////////////////////////////////////////////////////////////// OBD INIT

void BtObd::obd_init()
{
    Serial.print("\niniciando!");
    this->send_at_command(AT_DEFAULT_RESTORE);
    delay(200);
    this->send_at_command(AT_PROTOCOLO_AUTO);
    delay(200);
    Serial.print("\nComando AT enviados!");
}




////////////////////////////////////////////////////////////// OBD SEND PID
 void BtObd::obd_send_pid(String service, String pid)
 {
    String request = service + pid;
    SerialBT.println(request);
    Serial.print("\nRequest enviada: " + request);
 }


////////////////////////////////////////////////////////////// OBD READ RESPONSE

void BtObd::obd_read_response()
{
    this->read_response();
}




////////////////////////////////////////////////////////////// SERIAL DEBUG

String BtObd::obd_serial_debug(String pid)
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




////////////////////////////////////////////////////////////// SEND COMMAND
void BtObd::send_command(String request)
{

}



////////////////////////////////////////////////////////////// READ RESPONSE
void BtObd::read_response()
{
    if(SerialBT.available() > 0)
    {
        Serial.print("\nTamanho da resposta: ");
        Serial.print(sizeof(Serial.available()));

        char buff[sizeof(SerialBT.available()) + 1];

            for(int i = 0; i <= sizeof(buff); i++)
            {
                buff[i] = SerialBT.read();
            }

        Serial.print("\nBuffer do Serial Bluetooth: ");
        Serial.print(buff);
        
        
            for(int j = 0; j <= sizeof(buff); j++)
            {
                Serial.print("Byte [" + j);
                Serial.print("]");
                Serial.print(buff[j]);
            }

    }else
    {
        Serial.print("\nBuffer de resposta vazio...");
    }
    

}


////////////////////////////////////////////////////////////// SEND AT COMMAND
void BtObd::send_at_command(String at_comand)
{
    SerialBT.println(at_comand);
}