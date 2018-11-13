#include "PWM_IF.h"
#include "Arduino.h"
  
CPWMIF::CPWMIF(PWMID_e ID):
  m_ID(ID)
{
  //do nothing
}
CPWMIF::~CPWMIF(){
  m_tID = PWM1;
}

void CPWMIF::Init(unsigned int baud){
	// to do :
}

 bool CPWMIF::SetDutyCycle(unsigned char duty){
      // to do
 }
