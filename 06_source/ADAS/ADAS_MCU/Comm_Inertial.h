/**
* @file Comm_Inertial.h
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for inertial communication
* 
*/

#ifndef COMMS_INERTIAL_SYS_H

// ##### Includes #####
#include "ADAS_Types.h"

// Define constant for communication
#define COMMS_INERTIAL_SYS_H



// ##### Definitions #####
//!Define class for Inertial Communication
class CInertialComm
{
public:
    /// Constructor
    CInertialComm();
    /// Destructor
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
    ///Set constant maximum Send Buffer
    static const uint8_t MaxSndBuff = 10;
    ///Set constant maximum Receive Buffer
    static const uint8_t MaxRecBuff = 10;
    uint8_t m_sndBuff[MaxSndBuff];
    uint8_t m_rcvBuff[MaxRecBuff];
};

#endif /*COMMS_INERTIAL_SYS_H */
