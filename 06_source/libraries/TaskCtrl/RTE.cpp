#include "TaskCtrl.h"
#include "Arduino.h"
  
CRTE::CRTE()
{
  //do nothing
}
CRTE::~CRTE(){
  //do nothing
}

void CRTE::Init(void)
{

}

unsigned char CRTE::ReadData(void)
{
	return m_data;
}

void CRTE::WriteData(unsigned char& data)
{
	m_data = data;
}
