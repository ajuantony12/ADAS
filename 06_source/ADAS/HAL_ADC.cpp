#include "HAL_ADC.h"
#include <Arduino.h>
  
CADC::CADC()
{
  //do nothing
}
CADC::~CADC(){
}

void CADC::Init(void){
}

 uint16_t CADC::ReadADCValue(uint8_t pin){
      return analogRead(pin);
 }
