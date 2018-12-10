#include "Serial_IF.h"
#include "Arduino.h"

#define ATMEGA_BOARD
  
CSerial::CSerial(PortNum_e ID):
  m_PortID(ID),
  m_Port(NULL)
{
  //do nothing
}
CSerial::~CSerial(){
  m_PortID = Port1;
  m_Port = NULL;
}

void CSerial::Init(unsigned int baud, unsigned short time_out){

#ifdef ATMEGA_BOARD
    switch(m_PortID)
    {
    case  Port1:
      m_Port = &Serial1;
#else
      m_Port = &Serial;
#endif
#ifdef ATMEGA_BOARD
      break;
    case  Port2:
      m_Port = &Serial2;
      break;
    case  Port3:
      m_Port = &Serial3;
    default:
      m_Port = &Serial1;
    }
#endif
    m_Port->begin(baud, SERIAL_8E1);
    m_Port->setTimeout(time_out);
}

 bool CSerial::Send(char Buff[], uint8_t len){
    bool retVal = (len == m_Port->write(Buff, len));
    m_Port->flush();
    return retVal;
 }

bool CSerial::Read(char Buff[], uint8_t len)
{
    return (len == m_Port->readBytes(Buff, len));
}
void CSerial::SetBaudRate(unsigned int baud, unsigned short time_out)
{
    m_Port->end();
    m_Port->begin(baud, SERIAL_8E1);
    m_Port->setTimeout(time_out);
}
uint8_t CSerial::Available(void)
{
    return m_Port->available();
}