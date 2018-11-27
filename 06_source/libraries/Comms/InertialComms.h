/*H**********************************************************************
* FILENAME :        InterialComms.h             
*
* DESCRIPTION : Inertial system communication layer
*************************************************************************/

#include <ADAS_Types.h>

#ifndef COMMS_INERTIAL_SYS_H
#define COMMS_INERTIAL_SYS_H

class CInertialComm
{
public:
    CInertialComm();
    ~CInertialComm();
  /*
   * init
   */
   void Init(void);
  /*
   * Send
   */
  bool send(uint8_t buff[], uint8_t len);
  /*
   * recieve
   */
  uint8_t recieve(uint8_t buff[], uint8_t len);
private:
    static const uint8_t MaxSndBuff = 10;
    static const uint8_t MaxRecBuff = 10;
    uint8_t m_sndBuff[MaxSndBuff];
    uint8_t m_rcvBuff[MaxRecBuff];
};

#endif /*COMMS_INERTIAL_SYS_H */
