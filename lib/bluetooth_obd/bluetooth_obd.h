#ifndef BLUETOOTH_OBD_H
#define BLUETOOTH_OBD_H


#include "BluetoothSerial.h"
#include "pid.h"


 




class BtObd
{
   public:
        BtObd();
        void obd_init();
        void obd_send_pid(String service, String pid);
        void obd_read_response();
        String obd_serial_debug(String pid);

        String suported_pids_01_20  = "";
        String status_dtc           = "";
        String temp_engine          = "";
        String rpm                  = "";
        String speed                = "";
        String pos_acel             = "";
        String suported_pids_21_40  = "";
        String fuel_level           = "";
        String voltage_control_mo   = "";
        String temp_amb             = "";
        String fuel_type            = "";
        String fuel_rate            = "";
        String suported_pids_41_60  = "";
        String odometer             = "";
        
    
        
    private:
        void send_command(String request);
        void read_response();
        void send_at_command(String at_comand);
       
   
};










#endif

