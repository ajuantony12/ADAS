/*H**********************************************************************
* FILENAME :        Encoder_IF.h             
*
* DESCRIPTION : encoder Hardware interface
*************************************************************************/
#include "ADAS_Types.h"

#ifndef HAL_ENCODER_H
#define HAL_ENCODER_H

class CEncoder{
public:
  typedef enum {
    E1,
    E2
  }EncoderID_e;
  /*
   * Constructor
   */
  CEncoder(EncoderID_e ID);
  /*
   * Destructor
   */
  ~CEncoder();
  /*
   * Initialization
   */
  void Init();
  /*
   * read encoder value
   */
   unsigned int Read();
   /**
   * Reset the value to zero
   */
   bool reset();
 private:
   EncoderID_e m_ID;
   void EncISR(void);
  
};

#endif /*HAL_ENCODER_H*/
