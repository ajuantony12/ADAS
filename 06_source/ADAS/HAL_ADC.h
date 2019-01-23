/*H**********************************************************************
* FILENAME :        ADC.h             
*
* DESCRIPTION : ADC Hardware interface
*************************************************************************/

#include "ADAS_Types.h"

#ifndef HAL_ADC_H
#define HAL_ADC_H

class CADC{
public:
  /*
   * Constructor
   */
  CADC();
  /*
   * Destructor
   */
  ~CADC();
  /*
   * Initialization
   */
  void Init();
  /*
   * Read ADC Value
   */
   uint16_t ReadADCValue(uint8_t pin);
  
};

#endif /*HAL_ADC_H*/
