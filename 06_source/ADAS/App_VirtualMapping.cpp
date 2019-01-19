#include "App_VirtualMapping.h"
#include "ADAS_Debug.h"
#include <Arduino.h>

#define PLS_PF_MASK     ((uint16_t)0x1<<15)
#define PLS_WF_MASK     ((uint16_t)0x1<<14)

CVMapping::CVMapping(CNavigation& NAV, CPLSComms& plsComms, uint8_t activeCheckInt):
  m_NAV(NAV),
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
  //do nothing
}

void CVMapping::Run(void)
{
  uint16_t len;
  CPLSComms::Message_t msg;
  if (!m_NAV.isTurning())
  {
      if (m_plsComms.IsPFBreached())
      {
        if (m_plsComms.GetAsyncData(msg, len))
        {
            uint16_t len = OrganizeData(reinterpret_cast<uint16_t*>(msg.data), msg.len);
            if (searchforPFBreach(reinterpret_cast<uint16_t*>(msg.data), len))
            {
                //stop cart
            }
            else{
                calculateWallInfo(reinterpret_cast<uint16_t*>(msg.data), len, true);
            }
        }
      }
  } else if(m_NAV.isCornerMode())
  {
      if (m_plsComms.GetMeasurements(msg, len, true))
      {
          // send vertical distance
          //(uint16_t)msg.data[1]<<8 | msg.data[0];
      }
  }
  if (m_ActiveCheckInterval > m_RunCount)
  {
    DPRINTLN("Sending measurement request");
    if (m_plsComms.GetMeasurements(msg, len, false))
    {
      DPRINTLN("recieved measurement");
      DPRINTLN(len, HEX);
      uint16_t len = OrganizeData(reinterpret_cast<uint16_t*>(msg.data), msg.len);
      calculateWallInfo(reinterpret_cast<uint16_t*>(msg.data), len, false);
    }
    m_RunCount = 0;
  }
  m_RunCount++;
}

void CVMapping::Stop(void)
{
  //do nothing
}

uint16_t CVMapping::OrganizeData(uint16_t* data, const uint16_t len)
{
    uint16_t retVal = (data[0]<<8) | (data[0] >> 8);
    //DPRINT("LEN = ");
    //DPRINT(static_cast<uint8_t>(len>>8), HEX);
    //DPRINTLN(static_cast<uint8_t>(len & 0xff), HEX);
    uint16_t* temp = &data[1];
    if ((len-2) ==(retVal << 1))
    {
        for (uint16_t i=0; i < retVal; i++)
        {
            temp[i] = (temp[i]<<8) | (temp[i] >> 8);
            //DPRINT(static_cast<uint8_t>(temp>>8), HEX);
            //DPRINTLN(static_cast<uint8_t>(temp & 0xff), HEX);
        }
    }
    else{
        retVal = 0;
    }
    memmove(data, temp, (len*2));
    return retVal;
}
bool CVMapping::searchforPFBreach(uint16_t* data, uint16_t len)
{
    bool retVal = false;
    uint8_t wfSeg = 0;
    uint16_t i;
    for (i=0; ((i < len) && (false == retVal)); i++)
    {
        retVal = (data[i] & PLS_PF_MASK) ? true : false;
        if ((false == retVal) && (wfSeg < PLS_WF_SEGEMENTS_RIGHT))
        {
            retVal = (data[i] & PLS_WF_MASK) ? true : false;
        }
    }
    DPRINTLN("field Breach detected");
    DPRINTLN(i, DEC);
    return retVal;
}
void CVMapping::calculateWallInfo(uint16_t* data, uint16_t len, bool wfStatus)
{
    
}