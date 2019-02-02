/**
* @file HAL_ADC.h
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief Hardware Abstraction Layer (HAL) to interface ADC
*
*/

#include "ADAS_Types.h"

#ifndef HAL_ADC_H
#define HAL_ADC_H

//! Hardware Abstraction Layer (HAL) ADC class
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
