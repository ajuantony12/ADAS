/*H**********************************************************************
* FILENAME :        Encoder_IF.h             
*
* DESCRIPTION : encoder Hardware interface
*************************************************************************/

#ifndef HAL_ENCODER_IF
#define HAL_ENCODER_IF

class CEncoder_IF{
public:
  typedef enum {
    E1,
    E2
  }EncoderID_e;
  /*
   * Constructor
   */
  CEncoder_IF(EncoderID_e ID);
  /*
   * Destructor
   */
  ~CEncoder_IF();
  /*
   * Initialization
   */
  void Init();
  /*
   * Send Data
   */
   bool Read(unsigned short& value);
   /**
   * Reset the value to zero
   */
   bool reset();
 private:
   EncoderID_e m_ID;
  
};

#endif /*HAL_ENCODER_IF*/
