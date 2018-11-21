#include "InertialComms.h"
#include "Arduino.h"
  
CInertialComm::CInertialComm()
{
  //do nothing
}
CInertialComm::~CInertialComm(){
  //do nothing
}

void CInertialComm::Init(void)
{

}

bool CInertialComm::send(uint8_t buff[], uint8_t len)
{
    //do nothing
    return true;
}

uint8_t CInertialComm::recieve(uint8_t buff[], uint8_t len)
{
    //do nothing
    return 0;
}
