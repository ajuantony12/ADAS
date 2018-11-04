/*H**********************************************************************
* FILENAME :        Serial.h             
*
* DESCRIPTION : Serial Hardware interface
*************************************************************************/

#ifndef SERIAL_IF_ADAS
#define SERIAL_IF_ADAS

class CSerialIF{
public:
  typedef enum {
    Port1,
    Port2,
    Port3
  }PortNum_e;
  /*
   * Constructor
   */
  CSerialIF(PortNum_e ID);
  /*
   * Destructor
   */
  ~CSerialIF();
  /*
   * Initialization
   */
  void Init(unsigned int baud);
  /*
   * Send Data
   */
   bool Send(char Buff[], unsigned short len);
 private:
   PortNum_e m_PortID;
  
};

#endif
