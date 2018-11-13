#include "Encoder_IF.h"
#include "Arduino.h"
  
CEncoder_IF::CEncoder_IF(PortNum_e ID):
  m_ID(ID)
{
  //do nothing
}
CEncoder_IF::~CEncoder_IF(){
  m_PortID = Port1;
}

void CEncoder_IF::Init(){
	//to do
}

bool Read(unsigned short& value)
{
	//to do
	return true;
}

bool reset()
{
	//to do 
	return true;
}
