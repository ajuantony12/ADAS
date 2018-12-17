#include "App_VirtualMapping.h"
#include "ADAS_Debug.h"
#include <Arduino.h>

CVMapping::CVMapping(CPLSComms& plsComms, uint8_t activeCheckInt):
  m_plsComms(plsComms),
  m_RunCount(0),
  m_ActiveCheckInterval(activeCheckInt)
{
  //do nothing
}
CVMapping::~CVMapping() {
  //do nothing
}
void CVMapping::Init(void)
{
  m_plsComms.Init();
}

void CVMapping::Run(void)
{
  uint16_t len;
  uint8_t* buff;
  if (m_ActiveCheckInterval > m_RunCount)
  {
    DPRINTLN("Sending measurement request");
    if (m_plsComms.GetMeasurements(buff, len))
    {
      DPRINTLN("recieved measurement");
      DPRINTLN(len, HEX);
    }
  }
}

void CVMapping::Stop(void)
{
  //do nothing
}
