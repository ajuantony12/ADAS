#include "Serial_IF.h"
#include "Arduino.h"
  
CSerialIF::CSerialIF(PortNum_e ID):
  m_PortID(ID)
{
  //do nothing
}
CSerialIF::~CSerialIF(){
  m_PortID = Port1;
}

void CSerialIF::Init(unsigned int baud){

#if 0
  switch(m_PortID)
  {
    case  Port1:
#endif
      Serial.begin(baud);
#if 0      
      break;
    case  Port2:
      Serial2.begin(baud);
      break;
    case  Port3:
      Serial3.begin(baud);
    default:
      //do nothing
  }
#endif
}

 bool CSerialIF::Send(char Buff[], unsigned short len){
      return (len == Serial.write(Buff, len));
 }
