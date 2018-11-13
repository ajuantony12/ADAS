#include "I2C_IF.h"
#include "Arduino.h"
  
CI2C_IF::CI2C_IF(PortNum_e ID):
  m_ID(ID)
{
  //do nothing
}
CI2C_IF::~CI2C_IF(){
  m_PortID = Port1;
}

void CI2C_IF::Init(){
	//to do
}

bool CI2C_IF::send(unsigned char& value){
	//to do
	return true;
}

bool CI2C_IF::rcv(unsigned char& value)
{
	//to do
	return true
}
