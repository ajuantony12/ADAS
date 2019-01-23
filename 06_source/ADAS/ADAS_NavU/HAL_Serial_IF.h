/*H**********************************************************************
* FILENAME :        Serial.h             
*
* DESCRIPTION : Serial Hardware Header
*************************************************************************/
#include "ADAS_Types.h"

#ifndef HAL_SERIAL_H
#define HAL_SERIAL_H

class CSerial
{
public:
  typedef enum {
    S1,
    S2
  }PortID_e;
  /*
   * Constructor
   */
  CSerial(PortID_e i_ID, uint16_t i_bufLen);
  /*
   * Destructor
   */
  ~CSerial();
  /*
   * Initialization
   */
   void Init(void);
  /*
   * Send Data
   */
   bool Send(char Buff[], uint8_t len);
   bool Available(void);
   uint16_t GetDataLen(void);
   bool GetData(uint8_t* data, uint16_t len);
   void ReleaseBuffer(void);
   void SerialISRcommPLS(void);
   void SerialISRcommICC(void);
 private:
   uint8_t* m_rxBuffer;
   PortID_e m_ID;
   const uint16_t m_rxBufLen;
    volatile uint16_t m_rxMsgLen;
    volatile uint16_t m_rxBufferPointer;
    volatile bool m_rxBufferFull;
    volatile bool m_rxBufferRdy;
  
};

#endif /*HAL_SERIAL_1_H*/
