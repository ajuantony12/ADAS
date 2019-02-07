/**
* @file ADAS_Cfg.h
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief This file contains the configuration of the vehicle
*
*/


#ifndef ADAS_CFG_H
#define ADAS_CFG_H
/**one time configuration*/
//#define ONE_TIME_CFG
#define ADAS_DEBUG

//PLS Buffer
/** Size of PLS receiving buffer in bits */
#define PLS_RCV_BUFF_SIZE                                   500U
/** Size of PLS sending buffer in bits */
#define PLS_SND_BUFF_SIZE                                   50U

//ICC Buffer
/** Size of ICC receiving buffer in bits */
#define ICC_RCV_BUFF_SIZE                                   ICC_LEN
/** Size of ICC sending buffer in bits */
#define ICC_SND_BUFF_SIZE                                   ICC_LEN*10
//Virtual mapping Buffer
#define MAX_MES_BUFF_SIZE                                   (20)

//Task list
/**Maximum number of tasks*/
#define MAX_NUM_TASK                                    5U

//Serial Port
/** Definition of serial1 baudrate in bit/s */
#define SERIAL1_BAUD                                    9600
/** Definition of UBRRn value according to ::SERIAL1_BAUD */
#define SERIAL1_BAUD_PRESCALE                           (((F_CPU / (SERIAL1_BAUD * 16UL))) - 1)
/** Definition of serial2 baudrate in bit/s */
#define SERIAL2_BAUD                                    9600
/** Definition of UBRRn value according to ::SERIAL2_BAUD */
#define SERIAL2_BAUD_PRESCALE                           (((F_CPU / (SERIAL2_BAUD * 16UL))) - 1)

//Pins
/** PWM for Left Motor*/
#define PIN_MTR_L_PWM                                   11
/** PWM for Right Motor*/
#define PIN_MTR_R_PWM                                   12
/** Enable for Right and Left Motor*/
#define PIN_ENABLE                                      52
/** Direction for Left Motor*/
#define PIN_DIRECTION_L                                 50
/** Direction for Right Motor*/
#define PIN_DIRECTION_R                                 48
/** Encoder Interrupt for Right Motor*/
#define PIN_ENC_R                                        2
/** Encoder Interrupt for Left Motor*/
#define PIN_ENC_L                                        3
/** External Start Button*/
#define Btn_Start                                       46
/** Voltage Level of Battery*/
#define PIN_VBAT                                        A0


//Navigation parameters
/** Navigation parameter: Desired offset to wall on right side in cm. */
#define NAV_SET_OFFSET                                  80  // in cm
/** Navigation parameter: Allowed tolerance (+-) of offset to wall on right side in cm. */
#define NAV_TOL_OFFSET                                  5   // in cm
/** Navigation parameter: Allowed tolerance (+-) of angle in degrees */
#define NAV_TOL_ANGLE_DRIVE                             5   // degrees


//Inter-Controller Communication Protocol
/** Inter-Controller Communication: Definition of first start byte */
#define ICC_STX1                                        0xff
/** Inter-Controller Communication: Definition of second start byte */
#define ICC_STX2                                        0xff
/** Inter-Controller Communication: Definition of stop byte */
#define ICC_TTX                                         0xee
/** Inter-Controller Communication: Definition of length of message (including STXs and TTX) */
#define ICC_LEN                                         6U // including STXs and TTX
/** Inter-Controller Communication: Command to drive continuous */
#define ICC_CMD_CONT_DRIVE_IN                           0x01
/** Inter-Controller Communication: Command to do an emergency stop */
#define ICC_CMD_EMER_STOP                               0x02
/** Inter-Controller Communication: Command to do a soft stop */
#define ICC_CMD_SOFT_STOP                               0x03
/** Inter-Controller Communication: Command to drive a distance */
#define ICC_CMD_DRIVE_DIST                              0x04
/** Inter-Controller Communication: Command to rotate by angle */
#define ICC_CMD_ROT_ANGLE                               0x05
/** Inter-Controller Communication: Command to set speed */
#define ICC_CMD_SET_SPEED                               0x06
/** Inter-Controller Communication: Command to pause movement */
#define ICC_CMD_PAUSE_DRIVE                             0x07
/** Inter-Controller Communication: Command to continue movement */
#define ICC_CMD_CONT_DRIVE                              0x08
/** Inter-Controller Communication: Command to acknowledge message */
#define ICC_CMD_FB_ACK                                  0x01
/** Inter-Controller Communication: Command to acknowledge reached distance */
#define ICC_CMD_FB_DIST                                 0x02
/** Inter-Controller Communication: Command to acknowledge reached rotation */
#define ICC_CMD_FB_ROT                                  0x03

/** PLS | telegram start byte*/
#define SICK_STX                                        0x02
/** PLS | telegram reciever Address*/
#define SICK_DESTR                                      0x80
/** PLS | telegram acknowledgement byte*/
#define SICK_ACK                                        0x06
//Environmetal parameters
/** Definition of gain of the battery voltage monitor in mV/sample */
#define ENV_VBAT_GAIN                                   33 // mV/sample (33.7*1000/1024)
/** Definition of offset of battery voltage in mV */
#define ENV_VBAT_OFF                                    0  // mV
/** Definition of low voltage battery value */
#define ENV_VBAT_LOW                                    22000 // mV
/** Definition of critical voltage battery value */
#define ENV_VBAT_CRI                                    20000 // mV

//PID parameters
/** Definition of Proportional Gain */
#define KP_VALUE                                        2.5
/** Definition of Integral Gain */
#define KI_VALUE                                        2.5
/** Definition of Differential Gain */
#define KD_VALUE                                        0.0
/** Definition of clock rate*/
#define HZ_VALUE                                        10.0
/** Definition of Output Size */
#define OUTPUT_BITS                                     16
/** Definition of Output Signed/Unsigned */
#define OUTPUT_SIGNED                                   false

#endif /*ADAS_CFG_H*/
