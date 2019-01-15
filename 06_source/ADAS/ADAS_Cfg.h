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
//IPC Buffer
#define IPC_RCV_BUFF_SIZE                                   10U
//Virtual mapping Buffer
#define MAX_MES_BUFF_SIZE                                   (20)
//Task list
#define MAX_NUM_TASK                                    5U

//Serial Port
#define SERIAL1_BAUD                                    9600
#define SERIAL1_BAUD_PRESCALE                           (((F_CPU / (SERIAL1_BAUD * 16UL))) - 1)
#define SERIAL2_BAUD                                    9600
#define SERIAL2_BAUD_PRESCALE                           (((F_CPU / (SERIAL2_BAUD * 16UL))) - 1)

//PLS Comms COnstant
#define SICK_STX                                        0x02
#define SICK_DESTR                                      0x80

#endif /*ADAS_CFG_H*/
