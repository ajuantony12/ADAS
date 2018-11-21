#include "I2C.h"
#include "Arduino.h"
  
CI2C::CI2C(I2CID_e ID):
  m_ID(ID)
{
  //do nothing
}
CI2C::~CI2C(){
  m_ID = I2C1;
}

void CI2C::Init(){
	//to do
}

bool CI2C::send(uint8_t* data, uint8_t len){
	//to do
	return true;
}

bool CI2C::rcv(uint8_t* data, uint8_t len)
{
	//to do
	return true;
}
