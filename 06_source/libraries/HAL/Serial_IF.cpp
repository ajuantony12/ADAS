#include "Serial_IF.h"
#include "Arduino.h"
  
CSerial::CSerial(PortNum_e ID):
  m_PortID(ID)
{
  //do nothing
}
CSerial::~CSerial(){
  m_PortID = Port1;
}

void CSerial::Init(unsigned int baud, unsigned short time_out){

#if 0
  switch(m_PortID)
  {
    case  Port1:
#else
      Serial.begin(baud);
      Serial.setTimeout(time_out);
#endif
#if 0      
      break;
    case  Port2:
      Serial2.begin(baud);
      Serial2.setTimeout(time_out);
      break;
    case  Port3:
      Serial3.begin(baud);
      Serial3.setTimeout(time_out);
    default:
      //do nothing
  }
#endif
}

 bool CSerial::Send(char Buff[], uint8_t len){
      return (len == Serial.write(Buff, len));
 }

bool CSerial::Read(char Buff[], uint8_t len)
{
    return true;
}
uint8_t Available(void)
{
    return 0;
}