/**
* @file ADAS_Cfg.h
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief This file contains the configuration of the vehicle
*
*/

#ifndef ADAS_CFG_H
#define ADAS_CFG_H

/** Enable this definition for debug print outs */
//#define ADAS_DEBUG
/** Enable this definition for debug2 print outs */
#define ADAS_DEBUG2

//PLS Buffer
/** Size of PLS receiving buffer in bytes */
#define PLS_RCV_BUFF_SIZE                                   500U
/** Size of PLS sending buffer in bytes */
#define PLS_SND_BUFF_SIZE                                   50U
	
//ICC Buffer
/** Size of ICC receiving buffer in bytes*/
#define ICC_RCV_BUFF_SIZE                                   ICC_LEN
/** Size of ICC sending buffer in bytes */
#define ICC_SND_BUFF_SIZE                                   ICC_LEN*10
//Task list
/** Maximum number of possible tasks handled by task controller */
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

/** Maximum number of segments on the right side of Warning field rectangular field*/
#define PLS_WF_SEGEMENTS_RIGHT                         63U
/** Left corner segment position*/
#define PLS_WF_SEGEMENTS_LEFT_CORNER                          117U
/** Second point segment for wall angle detection 50 cm forward on right side*/
#define PLS_WALL_DETECTION_2_POINT                     45U
/** Maximum allowed vertical distance for free run*/
#define PLS_MAX_VERTICAL_DIST                          100U
/** Tolerance for right wall offset check*/
#define PLS_RIGHT_OFFSET_TOLERANCE                      5U
/** Maximum Number of warning field segment 
*   Defined through Desktop client of PLS 
*/
#define PLS_WF_SEGEMENTS_MAX                           180U
/** PLS | telegram start byte*/
#define SICK_STX                                        0x02
/** PLS | telegram reciever Address*/
#define SICK_DESTR                                      0x80
/** PLS | telegram acknowledgement byte*/
#define SICK_ACK                                        0x06
/** PLS | telegram not acknowledgement byte*/
#define SICK_NAK                                        0x15
//one time configuration
//#define ONE_TIME_CFG

//Pins
/** Definition of analog input pin for battery voltage monitor */
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

//Environmental parameters
/** Definition of gain of the battery voltage monitor in mV/sample */
#define ENV_VBAT_GAIN                                   32 // mV/sample (25.05V*1000/778)  
/** Definition of offset of battery voltage in mV */
#define ENV_VBAT_OFF                                    0  // mV
/** Definition of low voltage battery value */
#define ENV_VBAT_LOW                                    22000 // mV
/** Definition of critical voltage battery value */
#define ENV_VBAT_CRI                                    20000 // mV


#endif /*ADAS_CFG_H*/
