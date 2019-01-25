#include <Arduino.h>
#include "ADAS_Debug.h"
#include "App_VirtualMapping.h"

#define PLS_PF_MASK         ((uint16_t)0x1<<15)
#define PLS_WF_MASK         ((uint16_t)0x1<<14)
#define PLS_DIST_MASK       0x1FFFU
// Scaling factor for sine and cosine tables
#define TRIG_NORM_SHIFT  12
#define CORDIC_SHIFT  8
#define CORDIC_STAGES  10

// Table for SINE(alpha)*SINE_NORM from 0 deg to 180 deg in 1 deg steps
// Divide/shift results of multiplication with SINE_NORM to get final result
static const sint16_t sine_tab[181] =
{ 0, 71, 143, 214, 286, 357, 428, 499, 570, 641, 711, 782, 852, 921, 991,
  1060, 1129, 1198, 1266, 1334, 1401, 1468, 1534, 1600, 1666, 1731, 1796,
  1860, 1923, 1986, 2048, 2110, 2171, 2231, 2290, 2349, 2408, 2465, 2522,
  2578, 2633, 2687, 2741, 2793, 2845, 2896, 2946, 2996, 3044, 3091, 3138,
  3183, 3228, 3271, 3314, 3355, 3396, 3435, 3474, 3511, 3547, 3582, 3617,
  3650, 3681, 3712, 3742, 3770, 3798, 3824, 3849, 3873, 3896, 3917, 3937,
  3956, 3974, 3991, 4006, 4021, 4034, 4046, 4056, 4065, 4074, 4080, 4086,
  4090, 4094, 4095, 4096, 4095, 4094, 4090, 4086, 4080, 4074, 4065, 4056,
  4046, 4034, 4021, 4006, 3991, 3974, 3956, 3937, 3917, 3896, 3873, 3849,
  3824, 3798, 3770, 3742, 3712, 3681, 3650, 3617, 3582, 3547, 3511, 3474,
  3435, 3396, 3355, 3314, 3271, 3228, 3183, 3138, 3091, 3044, 2996, 2946,
  2896, 2845, 2793, 2741, 2687, 2633, 2578, 2522, 2465, 2408, 2349, 2290,
  2231, 2171, 2110, 2048, 1986, 1923, 1860, 1796, 1731, 1666, 1600, 1534,
  1468, 1401, 1334, 1266, 1198, 1129, 1060, 991, 921, 852, 782, 711, 641,
  570, 499, 428, 357, 286, 214, 143, 71, 0
};

// Table for COSINE(alpha)*COS_NORM from 0 deg to 180 deg in 1 deg steps
// Divide/shift results of multiplication with COS_NORM to get final result
static const sint16_t cos_tab[181] =
{ 4096, 4095, 4094, 4090, 4086, 4080, 4074, 4065, 4056, 4046, 4034, 4021,
  4006, 3991, 3974, 3956, 3937, 3917, 3896, 3873, 3849, 3824, 3798, 3770,
  3742, 3712, 3681, 3650, 3617, 3582, 3547, 3511, 3474, 3435, 3396, 3355,
  3314, 3271, 3228, 3183, 3138, 3091, 3044, 2996, 2946, 2896, 2845, 2793,
  2741, 2687, 2633, 2578, 2522, 2465, 2408, 2349, 2290, 2231, 2171, 2110,
  2048, 1986, 1923, 1860, 1796, 1731, 1666, 1600, 1534, 1468, 1401, 1334,
  1266, 1198, 1129, 1060, 991, 921, 852, 782, 711, 641, 570, 499, 428,
  357, 286, 214, 143, 71, 0, -71, -143, -214, -286, -357, -428, -499,
  -570, -641, -711, -782, -852, -921, -991, -1060, -1129, -1198, -1266,
  -1334, -1401, -1468, -1534, -1600, -1666, -1731, -1796, -1860, -1923,
  -1986, -2048, -2110, -2171, -2231, -2290, -2349, -2408, -2465, -2522,
  -2578, -2633, -2687, -2741, -2793, -2845, -2896, -2946, -2996, -3044,
  -3091, -3138, -3183, -3228, -3271, -3314, -3355, -3396, -3435, -3474,
  -3511, -3547, -3582, -3617, -3650, -3681, -3712, -3742, -3770, -3798,
  -3824, -3849, -3873, -3896, -3917, -3937, -3956, -3974, -3991, -4006,
  -4021, -4034, -4046, -4056, -4065, -4074, -4080, -4086, -4090, -4094,
  -4095, -4096
};

static const sint16_t atan_cordic_table[CORDIC_STAGES] =
{
    11520, 6800, 3593, 1824, 915, 458, 229, 114, 57, 28
};

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
          DPRINTLN(msg.len);
//          if (1 == temp[90]){
//              DPRINT("Vertical Distance = ");
//              DPRINTLN(temp[1], DEC);
//          }
              m_NAV.setPLSdata(temp[1]& PLS_DIST_MASK, 0, temp[91]& PLS_DIST_MASK);
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
                m_NAV.pauseDrive();
            }
            DPRINT("Data Length ");
            DPRINTLN(len, DEC);
            if(PLS_WF_SEGEMENTS_MAX == len) 
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
bool CVMapping::searchforPFBreach(uint16_t* data, uint16_t len, bool& pfStatus)
{
    bool retVal = false;
    uint8_t wfSeg = 0;
    uint16_t i;
    DPRINTLN(" ");
    for (i=0; i < len; i++)
    {
        DPRINT(data[i], HEX);
        DPRINT(" ");
        if (false == pfStatus){
            pfStatus = (data[i] & PLS_PF_MASK) ? true : false;
        }
        if ((false == retVal) && (i < PLS_WF_SEGEMENTS_RIGHT))
        {
            retVal = (data[i] & PLS_WF_MASK) ? true : false;
        }
    }
    return (retVal || pfStatus);
}
void CVMapping::calculateWallInfo(uint16_t* data, uint16_t len, bool wfStatus, bool pfStatus)
{
    uint16_t VDistance = PLS_MAX_VERTICAL_DIST;
    sint16_t offsetx0 = data[0] & PLS_DIST_MASK;
    sint16_t offsetx180 = data[PLS_WF_SEGEMENTS_MAX - 1] & PLS_DIST_MASK;
    int16_t wallAngleCordic = 0;
    if (pfStatus){
        DPRINTLN("PF Breach halt");
        return;
    } else if (wfStatus)
    {
        DPRINTLN("WF BREACH checking");
        sint16_t temp = 0;
        sint16_t tempy = 0;
        uint16_t index = PLS_WF_SEGEMENTS_RIGHT;
        uint8_t angle = 0;
        for (; index < PLS_WF_SEGEMENTS_LEFT; index++)
        {
            angle = (index > 90)? (180 - index) : (index);
            if (data[index] & PLS_WF_MASK)
            {
                temp = static_cast<sint16_t>((static_cast<sint32_t>(data[index] & PLS_DIST_MASK) * sine_tab[index]) >> TRIG_NORM_SHIFT);
                if (temp < VDistance)
                {
                    VDistance = temp;
                    DPRINTLN(VDistance, DEC);
                }
            }
        }
        DPRINT("VDistance ");
        DPRINTLN(VDistance, DEC);
        index = 0;
        sint16_t offsetx1 = offsetx0;
        sint16_t offsety = 0;
        DPRINTLN("WF BREACH RIGHT checking");
        for (int i = 1; i < PLS_WF_SEGEMENTS_RIGHT; i++)
        {
            if (PLS_WF_MASK & data[i]){
                angle = (i > 90)? (180 - i) : (i);
                temp = static_cast<sint16_t>((static_cast<sint32_t>(data[i] & PLS_DIST_MASK) * cos_tab[angle]) >> TRIG_NORM_SHIFT);
                tempy = static_cast<sint16_t>((static_cast<sint32_t>(data[i] & PLS_DIST_MASK) * sine_tab[angle]) >> TRIG_NORM_SHIFT);
                if ((temp < offsetx1) && (tempy < VDistance))
                {
                    DPRINTLN(" ");
                    DPRINT(i, DEC);
                    DPRINT("-");
                    DPRINT(temp, DEC);
                    DPRINT("|");
                    DPRINTLN(tempy, DEC);
                    index = i;
                    offsetx1 = temp;
                    offsety = tempy;
                }
            }
        }
        if ((offsetx0 - offsetx1) > PLS_LEFT_OFFSET_TOLERANCE)
        {
            wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety)<<16, static_cast<sint32_t>(offsetx0 - offsetx1)<<16)>>8); 
            wallAngleCordic = 90 - wallAngleCordic;
            
            DPRINTLN(" ");
            DPRINT("MINIMUM right dist : ");
            DPRINT((data[index] & PLS_DIST_MASK), DEC);
            DPRINT(" | ");
            DPRINT(index, DEC);
            DPRINT(" | ");
            DPRINT(offsetx1, DEC);
            DPRINT(" | ");
            DPRINTLN(offsety, DEC);
        }
    }else{
        sint16_t offsetx1 = static_cast<sint16_t>((static_cast<sint32_t>(data[PLS_WALL_DETECTION_2_POINT] & PLS_DIST_MASK) * cos_tab[PLS_WALL_DETECTION_2_POINT]) >> TRIG_NORM_SHIFT);
        sint16_t offsety = static_cast<sint16_t>((static_cast<sint32_t>(data[PLS_WALL_DETECTION_2_POINT] & PLS_DIST_MASK) * sine_tab[PLS_WALL_DETECTION_2_POINT]) >> TRIG_NORM_SHIFT);
        DPRINT("2nd point ");
        DPRINT((data[PLS_WALL_DETECTION_2_POINT] & PLS_DIST_MASK), HEX);
        DPRINT(" | ");
        DPRINT(offsetx1, DEC);
        DPRINT(" | ");
        DPRINT(offsety, DEC);
        DPRINT(" | cos :");
        DPRINT(cos_tab[PLS_WALL_DETECTION_2_POINT], DEC);
        DPRINT(" | sin :");
        DPRINTLN(cos_tab[PLS_WALL_DETECTION_2_POINT], DEC);
        if (offsetx1 > offsetx0){
            DPRINTLN("need to turn to right");
            if ((offsetx1 - offsetx0) > PLS_LEFT_OFFSET_TOLERANCE){
                wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety)<<16, static_cast<sint32_t>(offsetx1 - offsetx0)<<16)>>8);
                wallAngleCordic = -90 + wallAngleCordic;
            }
        }else{
            DPRINTLN("need to turn to left");
            if ((offsetx0 - offsetx1) > PLS_LEFT_OFFSET_TOLERANCE){
                wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety)<<16, static_cast<sint32_t>(offsetx0 - offsetx1)<<16)>>8); 
                wallAngleCordic = 90 - wallAngleCordic;
            }
        }

    }
    DPRINT("VALUES : ");
    DPRINT(offsetx0, DEC);
    DPRINT("|");
    DPRINT(wallAngleCordic, DEC);
    DPRINT("|");
    DPRINT(VDistance, DEC);
    DPRINT("|");
    DPRINTLN(offsetx180, DEC);
    m_NAV.setPLSdata(offsetx0, wallAngleCordic, VDistance);
    m_NAV.continueDrive();
}

sint16_t CVMapping::CordicATan(sint32_t y, sint32_t x)
{
    sint32_t result = 0;
    sint32_t temp;
    sint16_t z = 0;
    /*DPRINT(x, DEC);
    DPRINT("|");
    DPRINT(y, DEC);
    DPRINT("|");
    DPRINTLN(z, DEC); */
    for (uint8_t i =0U; i < CORDIC_STAGES; i++)
    {
        temp = x;
        if (y < 0)
        {
            x = x - (y>>i);
            y = y + (temp>>i);
            z = z - atan_cordic_table[i];
/*           DPRINT("- |");
            DPRINTLN(atan_cordic_table[i]); */
        }
        else{
            x = x + (y>>i);
            y = y - (temp >>i);
            z = z + atan_cordic_table[i];
/*          DPRINT("+ |");
            DPRINTLN(atan_cordic_table[i]); */
        }
/*        DPRINT(x, DEC);
        DPRINT("|");
        DPRINT(y, DEC);
        DPRINT("|");
        DPRINTLN(z, DEC);*/
        if (0 == y)
        {
            break;
        }
    }
    return z;
}
