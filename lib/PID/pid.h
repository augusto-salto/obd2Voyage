#ifndef PID_H
#define PID_H

#define PID_LENGTH 2
//SERVICES
#define SERVICE_01  "01"
#define SERVICE_02  "02"

// PID´S - SERVICE 01
#define SUPORTED_PIDS_01_20     "00"   //00 
#define STATUS_DTC              "01"   //01 
#define TEMP_ENGINE             "05"   //05
#define RPM_ENGINE              "0C"   //OC 
#define VEHICLE_SPEED           "0D"   //OD          
#define POS_ACEL                "11"   //11  
#define SUPORTED_PIDS_21_40     "20"   //20 
#define SUPORTED_PIDS_41_60     "40"   //40       
#define FUEL_TYPE               "51"   //51 


// AT COMANDS
#define AT_CABECALHO_OFF    "AT H0"
#define AT_CABECALHO_ON     "AT H1"
#define AT_PROTOCOLO_AUTO   "AT SP 00"
#define AT_ECHO_ON          "AT E 1"
#define AT_ECHO_OFF         "AT E 0"
#define AT_RESET_ALL        "AT Z"
#define AT_FAST_RESET       "AT WS"
#define AT_DEFAULT_RESTORE  "AT D"
#define AT_BATERY_VOLTAGE   "AT RV"
#define AT_IB_10            "AT IB 10"


//AT RESPONSES

#define AT_SEARCHING "SEARCHING..."
#define AT_UNABLE_TO_CONECT "UNABLE TO CONNECT"









#endif