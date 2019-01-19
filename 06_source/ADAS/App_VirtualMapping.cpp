#include <math.h> 
#include <Arduino.h>
#include "ADAS_Debug.h"
#include "ADAS_TrigTable.h"
#include "App_VirtualMapping.h"

#define PLS_PF_MASK         ((uint16_t)0x1<<15)
#define PLS_WF_MASK         ((uint16_t)0x1<<14)
#define PLS_DIST_MASK       0x1FFFU

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
  
  if(m_NAV.isCornerMode())
  {
      if (m_plsComms.GetAsyncData(msg, len))
      {
          uint16_t* temp = reinterpret_cast<uint16_t*>(msg.data);
          if (1 == temp[0]){
              DPRINT("Vertical Distance = ");
              DPRINTLN(temp[1], DEC);
          }
      }
      DPRINTLN("in corner mode getting vertical distance");
      m_plsComms.RequestMeasurements(true);
  }else{
      if (m_plsComms.DataAvailable())
      {
        DPRINTLN("Data available");
        if (m_plsComms.GetAsyncData(msg, len))
        {
            bool wfstatus = false;
            bool pfStatus = false;
            uint16_t len = OrganizeData(reinterpret_cast<uint16_t*>(msg.data), msg.len);
            if (searchforPFBreach(reinterpret_cast<uint16_t*>(msg.data), len, pfStatus))
            {
                DPRINTLN("field Breach detected");
                wfstatus = true;
                //stop cart
            }
            if((PLS_WF_SEGEMENTS_MAX+1) == len) 
            {
                DPRINTLN("Calculate wall info");
                calculateWallInfo(reinterpret_cast<uint16_t*>(msg.data), len, wfstatus, pfStatus);
            }
        }
      }
      else{
        m_RunCount++;
      }
  }
  if (m_ActiveCheckInterval < m_RunCount)
  {
    DPRINTLN("Sending measurement request");
    m_plsComms.RequestMeasurements(false);
    m_RunCount = 0; 
  }else{
      //do nothing
  }
}

void CVMapping::Stop(void)
{
  //do nothing
}

uint16_t CVMapping::OrganizeData(uint16_t* data, const uint16_t len)
{
    uint16_t retVal = data[0];
    memmove(data, &data[1], (retVal*2));
    return retVal;
}
bool CVMapping::searchforPFBreach(uint16_t* data, uint16_t len, bool pfStatus)
{
    bool retVal = false;
    uint8_t wfSeg = 0;
    uint16_t i;
    for (i=0; ((i < len) && (false == retVal)); i++)
    {
        DPRINTLN(data[i], HEX);
        retVal = (data[i] & PLS_PF_MASK) ? true : false;
        if ((false == retVal) && (wfSeg < PLS_WF_SEGEMENTS_RIGHT))
        {
            retVal = (data[i] & PLS_WF_MASK) ? true : false;
        }else if (true == retVal){
            pfStatus = true;
        }
    }
    DPRINTLN(i, DEC);
    return retVal;
}
void CVMapping::calculateWallInfo(uint16_t* data, uint16_t len, bool wfStatus, bool pfStatus)
{
    uint16_t VDistance = PLS_MAX_VERTICAL_DIST;
    uint16_t offsetx0 = data[0] & PLS_DIST_MASK;
    uint16_t offsetx180 = data[180] & PLS_DIST_MASK;
    int16_t wallAngle = 0;
    if (pfStatus){
        DPRINTLN("PF Breach halt");
        return;
    } else if (wfStatus)
    {
        DPRINTLN("WF BREACH checking");
        uint16_t temp = 0;
        uint16_t index = PLS_WF_SEGEMENTS_RIGHT;
        for (; index < PLS_WF_SEGEMENTS_LEFT; index++)
        {
            if (data[index] & PLS_WF_MASK)
            {
                temp = ((data[index] & PLS_DIST_MASK) * sine_tab[index]) >> TRIG_NORM_SHIFT;
                DPRINT(temp, DEC);
                if (temp < VDistance)
                {
                    VDistance = temp;
                }
            }
            DPRINT("VDistance ");
            DPRINTLN(VDistance, DEC);
        }
        index = 0;
        uint16_t offsetx1 = offsetx0;
        DPRINTLN("WF BREACH RIGHT checking");
        for (int i = 1; i < PLS_WF_SEGEMENTS_RIGHT; i++)
        {
            temp = ((data[i] & PLS_DIST_MASK) * cos_tab[i]) >> TRIG_NORM_SHIFT;
            if (temp < offsetx1)
            {
                DPRINT(temp, DEC);
                index = i;
                offsetx1 = temp;
            }
        }
        DPRINT("MINIMUM right dist : ");
        DPRINTLN(offsetx1, DEC);
        if ((offsetx0 - offsetx1) > PLS_LEFT_OFFSET_TOLERANCE)
        {
            uint16_t offsety = ((data[index] & PLS_DIST_MASK) * sine_tab[index]) >> TRIG_NORM_SHIFT;
            wallAngle = 90 - static_cast<uint16_t>(atan2(offsety, offsetx0 - offsetx1));
        }
    }else{
        uint16_t offsetx1 = ((data[PLS_WALL_DETECTION_2_POINT] & PLS_DIST_MASK) * cos_tab[PLS_WALL_DETECTION_2_POINT]) >> TRIG_NORM_SHIFT;
        uint16_t offsety = ((data[PLS_WALL_DETECTION_2_POINT] & PLS_DIST_MASK) * sine_tab[PLS_WALL_DETECTION_2_POINT]) >> TRIG_NORM_SHIFT;
        if (offsetx1 > offsetx0){
            DPRINTLN("need to turn to right");
            if ((offsetx1 - offsetx0) > PLS_LEFT_OFFSET_TOLERANCE){
                wallAngle = - static_cast<uint16_t>(atan2(offsety, offsetx1 - offsetx0));
            }
        }else{
            DPRINTLN("need to turn to left");
            if ((offsetx0 - offsetx1) > PLS_LEFT_OFFSET_TOLERANCE){
                wallAngle = 90 - static_cast<uint16_t>(atan2(offsety, offsetx0 - offsetx1));
            }
        }

    }
    DPRINT("VALUES : ");
    DPRINT(offsetx0, DEC);
    DPRINT("|");
    DPRINT(wallAngle, DEC);
    DPRINT("|");
    DPRINT(VDistance, DEC);
    DPRINT("|");
    DPRINTLN(offsetx180, DEC);
}