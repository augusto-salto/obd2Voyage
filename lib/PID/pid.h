#ifndef PID_H
#define PID_H

#define PID_LENGTH 4
//SERVICES
#define SERVICE_01  "01"
#define SERVICE_02  "02"

// PID´S - SERVICE 01
#define SUPORTED_PIDS_01_20     "00 1"
#define STATUS_DTC              "01 1"
#define TEMP_ENGINE             "05 1"
#define RPM_ENGINE              "0C 1"
#define VEHICLE_SPEED           "0D 1"
#define POS_ACEL                "11 1"
#define SUPORTED_PIDS_21_40     "20 1"
#define FUEL_TANK_LEVEL_INPUT   "2F 1"
#define SUPORTED_PIDS_41_60     "40 1"
#define VOLTAGE_CONTROL_MODULE  "42 1"
#define TEMP_AMBIENTE           "46 1"
#define FUEL_TYPE               "51 1"
#define FUEL_RATE               "5E 1"
#define SUPORTED_PIDS_61_80     "60 1"
#define TEMP_ENGINE_COOLANT     "67 1"
#define ODOMETER                "A6 1"


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












#endif