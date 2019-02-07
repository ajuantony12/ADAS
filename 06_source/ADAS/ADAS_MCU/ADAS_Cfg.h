/*H**********************************************************************
  FILENAME :        ADAS_Cfg.h

  DESCRIPTION : configuration file for ADAS
*************************************************************************/

#ifndef ADAS_CFG_H
#define ADAS_CFG_H
//one time configuration
//#define ONE_TIME_CFG
#define ADAS_DEBUG

//ICC Buffer
#define ICC_RCV_BUFF_SIZE                                   ICC_LEN
#define ICC_SND_BUFF_SIZE                                   ICC_LEN*10

//Serial Port
#define SERIAL1_BAUD                                    9600
#define SERIAL1_BAUD_PRESCALE                           (((F_CPU / (SERIAL1_BAUD * 16UL))) - 1)
#define SERIAL2_BAUD                                    9600
#define SERIAL2_BAUD_PRESCALE                           (((F_CPU / (SERIAL2_BAUD * 16UL))) - 1)

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

/** PLS | telegram start byte*/
#define SICK_STX                                        0x02
/** PLS | telegram reciever Address*/
#define SICK_DESTR                                      0x80
/** PLS | telegram acknowledgement byte*/
#define SICK_ACK                                        0x06


#endif /*ADAS_CFG_H*/
