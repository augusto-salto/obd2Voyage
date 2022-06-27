#include "car.h"



void Car::init_car()
{
     //ble_client_setup();
}


void    Car::set_suported_pids_01(String value)
    {
        this->_suported_01_20 = value;
    }

void    Car::set_status_dts(String value)
    {
        this->_status_dtc = value;
    }

void    Car::set_temp_01(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);

    int valueInt = (int) strtol(buffer, 0, 16);

    this->_temp_engine = valueInt - 40;

    }

void    Car::set_rpm(String value)
    {  
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    char bufferA[3] = {0,0,0}, bufferB[3] = {0,0,0};

    bufferA[0] = buffer[0];
    bufferA[1] = buffer[1];
    bufferB[0] = buffer[3];
    bufferB[1] = buffer[4];

    int valueA, valueB;

    valueA = (int) strtol(bufferA, 0, 16);
    valueB = (int) strtol(bufferB, 0, 16);

    this->_rpm = (256 * valueA) + valueB;
    this->_rpm = this->_rpm / 4;

    Serial.print("\nRPM: ");
    Serial.print(this->_rpm);

    }

void    Car::set_speed(String value)
    {

    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    int valueInt = (int) strtol(buffer, 0, 16);
    this->_vehicle_speed = valueInt;

    }

void    Car::set_acel_pos(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    int valueInt = (int) strtol(buffer, 0, 16);
    this->_pos_acel = (100/255) * valueInt;

    }

void    Car::set_suported_pids_21(String value)
    {
        this->_suported_21_40 = value;
    }

void    Car::set_fuel_level(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    int valueInt = (int) strtol(buffer, 0, 16);
    this->_fuel_level = (100/255) * valueInt;
    }

void    Car::set_suported_pids_41(String value)
    {
        this->_suported_pids_41_60 = value;
    }

void    Car::set_voltage(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    char bufferA[3] = {0,0,0}, bufferB[3] = {0,0,0};

    bufferA[0] = buffer[0];
    bufferA[1] = buffer[1];
    bufferB[0] = buffer[3];
    bufferB[1] = buffer[4];

    int byteA, byteB;

    byteA = (int) strtol(bufferA, 0, 16);
    byteB = (int) strtol(bufferB, 0, 16);

    this->_voltage_ecu = (256 * byteA) + byteB;
    this->_voltage_ecu = this->_voltage_ecu/1000;
    }

void    Car::set_ambient_temp(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);

    int valueInt = (int) strtol(buffer, 0, 16);

    this->_temp_ambiente = valueInt - 40;       
    }

void    Car::set_fuel_type(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    int valueInt = (int) strtol(buffer, 0, 16);
    
    if(valueInt == 0)
    {
        this->_fuel_type = "Não definido";
    }
    else if(valueInt == 1)
    {
        this->_fuel_type = "Gasolina";
    }
    else if(valueInt == 2)
    {
        this->_fuel_type = "Metanol";
    }
    else if(valueInt == 3)
    {
        this->_fuel_type = "Etanol";
    }
    else if(valueInt > 3)
    {
        this->_fuel_type = "Outro";
    }

    }

void    Car::set_fuel_rate(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    char bufferA[3] = {0,0,0}, bufferB[3] = {0,0,0};

    bufferA[0] = buffer[0];
    bufferA[1] = buffer[1];
    bufferB[0] = buffer[3];
    bufferB[1] = buffer[4];

    int byteA, byteB;

    byteA = (int) strtol(bufferA, 0, 16);
    byteB = (int) strtol(bufferB, 0, 16);

    this->_fuel_rate = (256 * byteA) + byteB;
    this->_fuel_rate = this->_fuel_rate / 20;
    
    }

void    Car::set_suported_pids_61(String value)
    {
        this->_suported_61_80 = value;
    }

void    Car::set_coolant_temp(String value)
    {
        this->_temp_eng_coolant = 40; //TODO
    }

void    Car::set_odometer(String value)
    {
    value.trim(); 
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    char    bufferA[3] = {0,0,0}, bufferB[3] = {0,0,0}, 
            bufferC[3] = {0,0,0}, bufferD[3] = {0,0,0};

    bufferA[0] = buffer[0];
    bufferA[1] = buffer[1];
    bufferB[0] = buffer[3];
    bufferB[1] = buffer[4];
    bufferC[0] = buffer[6];
    bufferC[1] = buffer[7];
    bufferD[0] = buffer[9];
    bufferD[1] = buffer[10];

    int byteA, byteB, byteC, byteD;

    byteA = (int) strtol(bufferA, 0, 16);
    byteB = (int) strtol(bufferB, 0, 16);
    byteC = (int) strtol(bufferC, 0, 16);
    byteD = (int) strtol(bufferD, 0, 16);
    
    byteA = byteA * (pow(2,24) + 0.5);
    byteB = byteB * (pow(2,16) + 0.5);
    byteC = byteC * (pow(2,8) + 0.5);

    this->_odometer = byteA + byteB + byteC + byteD;
    this->_odometer = this->_odometer / 10;

    }

void Car::set_batery_voltage(String value)
{
    this->_batery_voltage = value.toInt();
}

int Car::get_rpm()
{
    return this->_rpm;
}

void    Car::set_connecting(bool connecting)
{
        this->_searching = connecting;

        if(connecting)
        {
            this->_unable = true;
        }

}

void    Car::set_running(bool running)
{
        this->_unable = running;
        if(running)
        {
            this->_searching = false;
        }

        if(!running)
        {
            this->_rpm = 0;
        }
}

bool    Car::is_connecting()
{
    return this->_searching;
}

bool    Car::is_running()
{   
    return this->_unable;
}