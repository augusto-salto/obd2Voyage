#ifndef CAR_H
#define CAR_H

#include "pid.h"
#include "Arduino.h"

 class Car
{
    private:
        String  _suported_01_20 = "";
        String  _status_dtc = "";
        int     _temp_engine = 0;
        int     _rpm = 0;
        int     _vehicle_speed = 0;
        int     _pos_acel = 0;
        String  _suported_21_40 = "";
        float   _voltage_ecu = 0.0;
        int     _temp_ambiente = 0;
        String  _fuel_type = "";
        float   _fuel_rate = 0.0;
        String  _suported_61_80;
        int     _temp_eng_coolant = 0;
        int     _odometer = 0;
        float   _batery_voltage = 0.0;
        bool    _searching = false;
        bool    _unable = true;
    
    public:
        void init_car();

        String  get_suported_pids_01();
        String  get_status_dts();
        int     get_temp_01();
        int     get_rpm();
        int     get_speed();
        int     get_acel_pos();
        String  get_suported_pids_21();
        float   get_voltage();
        int     get_ambient_temp();
        String  get_fuel_type();
        float   get_fuel_rate();
        String  get_suported_pids_61();
        int     get_coolant_temp();
        int     get_odometer();
        float   get_batery_volt();
        bool    is_connecting();
        bool    is_running();

        void    set_suported_pids_01(String value);
        void    set_status_dts(String value);
        void    set_temp_01(String value);
        void    set_rpm(String value);
        void    set_speed(String value);
        void    set_acel_pos(String value);
        void    set_suported_pids_21(String value);
        void    set_fuel_level(String value);
        void    set_suported_pids_41(String value);
        void    set_voltage(String value);
        void    set_ambient_temp(String value);
        void    set_fuel_type(String value);
        void    set_fuel_rate(String value);
        void    set_suported_pids_61(String value);
        void    set_coolant_temp(String value);
        void    set_odometer(String value);
        void    set_batery_voltage(String value);
        void    set_connecting(bool connecting);
        void    set_running(bool running);

};








#endif