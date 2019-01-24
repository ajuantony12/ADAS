/*H**********************************************************************
  FILENAME :        ADAS_Cfg.h

  DESCRIPTION : configuration file for ADAS
*************************************************************************/

#ifndef ADAS_CFG_H
#define ADAS_CFG_H
//one time configuration
//#define ONE_TIME_CFG
#define ADAS_DEBUG

//PLS Buffer
#define PLS_RCV_BUFF_SIZE                                   500U
#define PLS_SND_BUFF_SIZE                                   50U
//ICC Buffer
#define ICC_RCV_BUFF_SIZE                                   ICC_LEN
#define ICC_SND_BUFF_SIZE                                   ICC_LEN*10
//Virtual mapping Buffer
#define MAX_MES_BUFF_SIZE                                   (20)
//Task list
#define MAX_NUM_TASK                                    5U

//Serial Port
#define SERIAL1_BAUD                                    9600
#define SERIAL1_BAUD_PRESCALE                           (((F_CPU / (SERIAL1_BAUD * 16UL))) - 1)
#define SERIAL2_BAUD                                    9600
#define SERIAL2_BAUD_PRESCALE                           (((F_CPU / (SERIAL2_BAUD * 16UL))) - 1)

//Number of segements on right
#define PLS_WF_SEGEMENTS_RIGHT                         63U
#define PLS_WF_SEGEMENTS_LEFT                          117U
#define PLS_WALL_DETECTION_2_POINT                     45U
#define PLS_MAX_VERTICAL_DIST                          100U
#define PLS_LEFT_OFFSET_TOLERANCE                      5U
#define PLS_WF_SEGEMENTS_MAX                           180U

//PLS Comms Constant
#define SICK_STX                                        0x02
#define SICK_DESTR                                      0x80
#define SICK_ACK                                        0x06
#define SICK_NAK                                        0x15

//Pins
#define PIN_MTR_L_PWM                                   11
#define PIN_MTR_R_PWM                                   12
#define PIN_ENABLE                                      52
#define PIN_DIRECTION_L                                 50
#define PIN_DIRECTION_R                                 48
#define PIN_ENC_R                                        2
#define PIN_ENC_L                                        3
#define Btn_Start                                       46
#define PIN_VBAT                                        A0


//Navigation parameters
#define NAV_SET_OFFSET                                  80  // in cm
#define NAV_TOL_OFFSET                                  5   // in cm
#define NAV_TOL_ANGLE_DRIVE                             5   // degrees

//Inter-Controller Communication Protocol
#define ICC_STX1                                        0xff
#define ICC_STX2                                        0xff
#define ICC_TTX                                         0xee
#define ICC_LEN                                         6U // including STXs and TTX
#define ICC_CMD_CONT_DRIVE_IN                           0x01
#define ICC_CMD_EMER_STOP                               0x02
#define ICC_CMD_SOFT_STOP                               0x03
#define ICC_CMD_DRIVE_DIST                              0x04
#define ICC_CMD_ROT_ANGLE                               0x05
#define ICC_CMD_SET_SPEED                               0x06
#define ICC_CMD_PAUSE_DRIVE                             0x07
#define ICC_CMD_CONT_DRIVE                              0x08
#define ICC_CMD_FB_ACK                                  0x01
#define ICC_CMD_FB_DIST                                 0x02
#define ICC_CMD_FB_ROT                                  0x03

//Environmetal parameters
#define ENV_VBAT_GAIN                                   32 // mV/sample (25.05V*1000/778)       
#define ENV_VBAT_OFF                                    0  // mV
#define ENV_VBAT_LOW                                    22000 // mV
#define ENV_VBAT_CRI                                    20000 // mV


#endif /*ADAS_CFG_H*/
