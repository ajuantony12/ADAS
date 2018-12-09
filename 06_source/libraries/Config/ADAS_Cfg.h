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
#define RCV_BUFF_SIZE                                   732U
#define SND_BUFF_SIZE                                   50U
//Virtual mapping Buffer
#define MAX_MES_BUFF_SIZE                               (20)
//Task list
#define MAX_NUM_TASK                                    5U

//Serial Port
#define SERIAL1_INITIAL_BAUD_RATE                       9600
#define SERIAL1_TIMEOUT                                 60

#endif /*ADAS_CFG_H*/
