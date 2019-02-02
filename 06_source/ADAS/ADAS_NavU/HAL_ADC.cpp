/**
* @file HAL_ADC.cpp
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief Hardware Abstraction Layer (HAL) to interface ADC
*
*/

#include "HAL_ADC.h"
#include <Arduino.h>
 
//! Constructor of CADC
CADC::CADC()
{
  //do nothing
}
//! Destructor of CADC
CADC::~CADC(){
}

//! Initialization function of CADC
void CADC::Init(void){
}

//! Function to read ADC value
/*!
	\param pin Pin number e.g. A0
	\return Returns current ADC value
*/
 uint16_t CADC::ReadADCValue(uint8_t pin){
      return analogRead(pin);
 }
