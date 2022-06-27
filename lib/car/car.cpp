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
        this->_vehicle_speed = value.toInt();
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