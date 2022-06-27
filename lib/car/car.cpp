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
        int buff = value.toInt();
        this->_temp_engine = buff - 40;
    }

void    Car::set_rpm(String value)
    {
    char buffer[value.length() + 1];
    value.toCharArray(buffer, value.length() + 1);
    char bufferA[3], bufferB[3];
    bufferA[0] = buffer[0];
    bufferA[1] = buffer[1];
    bufferB[0] = buffer[3];
    bufferB[1] = buffer[4];

    int valueA, valueB;

    valueA = atoi(bufferA);
    valueB = atoi(bufferB);

    this->_rpm = (256 * valueA) + valueB;
    this->_rpm = this->_rpm / 4;

    Serial.print("RPM: ");
    Serial.print(this->_rpm);

    }

void    Car::set_speed(String value)
    {
        
    }

void    Car::set_acel_pos(String value)
    {
        
    }

void    Car::set_suported_pids_21(String value)
    {
        
    }

void    Car::set_fuel_level(String value)
    {
        
    }

void    Car::set_suported_pids_41(String value)
    {
        
    }

void    Car::set_voltage(String value)
    {
        
    }

void    Car::set_ambient_temp(String value)
    {
        
    }

void    Car::set_fuel_type(String value)
    {
        
    }

void    Car::set_fuel_rate(String value)
    {
        
    }

void    Car::set_suported_pids_61(String value)
    {
        
    }

void    Car::set_coolant_temp(String value)
    {
        
    }

void    Car::set_odometer(String value)
    {
        
    }

void Car::set_batery_voltage(String value)
{
    
}