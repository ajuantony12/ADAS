/*H**********************************************************************
* FILENAME :        I2C.h             
*
* DESCRIPTION : I2C Hardware header
*************************************************************************/
#include <ADAS_Types.h>

#ifndef HAL_I2C_H
#define HAL_I2C_H

class CI2C{
public:
  typedef enum {
    I2C1,
  }I2CID_e;
  /*
   * Constructor
   */
  CI2C(I2CID_e ID);
  /*
   * Destructor
   */
  ~CI2C();
  /*
   * Initialization
   */
  void Init();
  /*
   * Send Data
   */
   bool send(uint8_t* data, uint8_t len);
   /**
   * recieve data
   */
   bool rcv(uint8_t* data, uint8_t len);
private:
    I2CID_e m_ID;
};

#endif /*HAL_I2C_H */
