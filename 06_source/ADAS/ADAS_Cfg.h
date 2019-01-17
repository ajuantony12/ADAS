/*H**********************************************************************
* FILENAME :        ADAS_Cfg.h             
*
* DESCRIPTION : configuration file for ADAS
*************************************************************************/

#ifndef ADAS_CFG_H
#define ADAS_CFG_H
//one time configuration
//#define ONE_TIME_CFG
#define ADAS_DEBUG

//PLS Buffer
#define RCV_BUFF_SIZE                                   720U
#define SND_BUFF_SIZE                                   50U
//Virtual mapping Buffer
#define MAX_MES_BUFF_SIZE                               (20)
//Task list
#define MAX_NUM_TASK                                    5U

//Serial Port
#define SERIAL1_INITIAL_BAUD_RATE                       9600
#define SERIAL1_TIMEOUT                                 500

//Pins
#define PIN_MTR_L_PWM                                   11
#define PIN_MTR_R_PWM                                   12
#define PIN_ENABLE                                      52
#define PIN_DIRECTION_L                                 50
#define PIN_DIRECTION_R                                 48
#define PIN_ENC_R                                        2
#define PIN_ENC_L                                        3
#define Btn_Start                                       46


//Navigation parameters
#define NAV_SET_OFFSET                                  80  // in cm
#define NAV_TOL_OFFSET                                  5   // in cm
#define NAV_TOL_ANGLE_DRIVE                             5   // degrees


#endif /*ADAS_CFG_H*/
