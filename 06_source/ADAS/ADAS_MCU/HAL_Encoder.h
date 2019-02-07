/*H**********************************************************************
* FILENAME :        Encoder_IF.h             
*
* DESCRIPTION : encoder Hardware interface
*************************************************************************/
// ##### Includes #####
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"

#ifndef HAL_ENCODER_H
#define HAL_ENCODER_H

//! Encoder Class
/**
 *
 * \class CEncoedr
 *
 * \brief   Class declaration for the encoder
**/
class CEncoder{
public:
  typedef enum {
    E1 = PIN_ENC_R,
    E2 = PIN_ENC_L
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
       Initialization of the encoder
    */
  void Init();
  /*
    read encoder peaks per 150ms
   */
   uint16_t ReadCount();
   /*
    read sum of encoder peaks
   */
   uint16_t ReadSum();
   /*
    Resets all values to zero
   */
   void reset(void);
    /*
    Encoder interrupt for counting the peaks
   */
   void EncISR(void);
 private:
   EncoderID_e m_ID;
   volatile uint16_t peaks, count, sum;
};

#endif /*HAL_ENCODER_H*/
