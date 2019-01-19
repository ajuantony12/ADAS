#include "HAL_Encoder.h"
#include <Arduino.h>
  
CEncoder::CEncoder(EncoderID_e ID):
  m_ID(ID)
{
  //do nothing
}
CEncoder::~CEncoder(){
  m_ID = E1;
}

void CEncoder::Init(){
	//to do
}

bool Read(uint16_t& value)
{
	//to do
	return true;
}

bool reset()
{
	//to do 
	return true;
}
