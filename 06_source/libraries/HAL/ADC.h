/*H**********************************************************************
* FILENAME :        ADC.h             
*
* DESCRIPTION : ADC Hardware interface
*************************************************************************/

#include <ADAS_Types.h>

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
   unsigned short ReadADCValue(void);
  
};

#endif /*HAL_ADC_H*/
