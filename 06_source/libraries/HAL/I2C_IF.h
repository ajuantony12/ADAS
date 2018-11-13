/*H**********************************************************************
* FILENAME :        I2C_IF.h             
*
* DESCRIPTION : I2C Hardware interface
*************************************************************************/

#ifndef HAL_I2C_IF
#define HAL_I2C_IF

class CI2C_IF{
public:
  /*
   * Constructor
   */
  CI2C_IF(EncoderID_e ID);
  /*
   * Destructor
   */
  ~CI2C_IF();
  /*
   * Initialization
   */
  void Init();
  /*
   * Send Data
   */
   bool send(unsigned char& value);
   /**
   * recieve data
   */
   bool rcv(unsigned char& value);
 
};

#endif /*HAL_I2C_IF */
