/**
  @file App_VirtualMapping.cpp
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Virtual mapping application source file

*/

#include <Arduino.h>
#include "ADAS_Debug.h"
#include "App_VirtualMapping.h"

/** Protective field status bit mask  */
#define PLS_PF_MASK         ((uint16_t)0x1<<15)
/** Warning field status bit mask  */
#define PLS_WF_MASK         ((uint16_t)0x1<<14)
/** Distance bits mask  */
#define PLS_DIST_MASK       0x1FFFU
/** Scaling for sine and cosine operations */
#define TRIG_NORM_SHIFT  12
/** scaling for cordic atanget operations*/
#define CORDIC_SHIFT  8
/** Number of stages for cordic algorithm */
#define CORDIC_STAGES  10

/*!
  sine table for 0 to 180 degrees in steps of 1 degree
*/
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

/*!
  cosine table for 0 to 180 degrees in steps of 1 degree
*/
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
/*!
  tangent values for cordic algorithm
*/
static const sint16_t atan_cordic_table[CORDIC_STAGES] =
{
  11520, 6800, 3593, 1824, 915, 458, 229, 114, 57, 28
};

//! Virtual mapping app class constructor.
CVMapping::CVMapping(CNavigation& NAV, CPLSComms& plsComms):
  m_NAV(NAV),
  m_plsComms(plsComms),
  m_driveState(false),
  m_dataRequested(false)
{
  //do nothing
}

//! Virtual mapping app class destructor.
CVMapping::~CVMapping() {
  //do nothing
}
//! Initialization function of virtual mapping Application
void CVMapping::Init(void)
{
  //do nothing
}
//! Run function of virtual mapping Application
void CVMapping::Run(void)
{
  uint16_t len;
  CPLSComms::Message_t msg;

  if (m_NAV.isCornerMode())
  {
    /** vehicle in corner mode
      ignore safety field breach while turning and use only vertical distance*/
    //check for data availability
    if (m_plsComms.DataAvailable())
    {
      DPRINTLN2("VM: data available VD");
      if (m_dataRequested)
      {
        // reset data request status flag
        m_dataRequested = false;
      }
      if (m_plsComms.GetAsyncData(msg, len))
      {
        // get 90 degree segment distance
        uint16_t* temp = reinterpret_cast<uint16_t*>(msg.data);
        DPRINTLN(msg.len);
        /*     //          if (1 == temp[90]){
            //              DPRINT("Vertical Distance = ");
            //              DPRINTLN(temp[1], DEC);
            //          } */
        //update vertical distance to Navigation app
        m_NAV.setPLSdata(temp[1]& PLS_DIST_MASK, 0, temp[91]& PLS_DIST_MASK);
      }
    }
    /* check data request flag and request for new data*/
    if (!m_dataRequested) {
      /*request for new data and set data request flag*/
      DPRINTLN2("VM: Sending request VD");
      m_plsComms.RequestMeasurements(true);
      m_dataRequested = true;
    }
    DPRINTLN("in corner mode getting vertical distance");
  } else {
    //check for data availability
    if (m_plsComms.DataAvailable())
    {
      DPRINTLN2("VM: Data available");
      if (m_dataRequested)
      {
        // reset data request status flag
        m_dataRequested = false;
      }
      DPRINTLN("Data available");
      if (m_plsComms.GetAsyncData(msg, len))
      {
        bool wfstatus = false;
        bool pfStatus = false;
        //organize data
        uint16_t len = OrganizeData(reinterpret_cast<uint16_t*>(msg.data), msg.len);
        // search field issues
        if (searchforPFBreach(reinterpret_cast<uint16_t*>(msg.data), len, pfStatus))
        {
          //set wfstatus flag
          DPRINTLN("field Breach detected");
          wfstatus = true;
          if (m_driveState)
          {
            //stop cart, if in drive mode true
            m_NAV.pauseDrive();
            m_driveState = false;
          }
        }
        DPRINT("Data Length ");
        DPRINTLN(len, DEC);
        //check if data recieved is complete
        if (PLS_WF_SEGEMENTS_MAX == len)
        {
          //calculate wall information
          DPRINTLN("Calculate wall info");
          calculateWallInfo(reinterpret_cast<uint16_t*>(msg.data), len, wfstatus, pfStatus);
        }
      }
    }
  }
  if (!m_dataRequested) {
    //request for dat and set dat request flag
    DPRINTLN2("VM: Sending request");
    m_plsComms.RequestMeasurements(false);
    m_dataRequested = true;
  }
}

//! Stop/shutdown function of virtual mapping Application
void CVMapping::Stop(void)
{
  //do nothing
}

//! organize the data buffer by removing unwanted fields
uint16_t CVMapping::OrganizeData(const uint16_t* data, const uint16_t len)
{
  //removing length bytes
  uint16_t retVal = data[0];
  memmove(data, &data[1], (retVal * 2));
  return retVal;
}
//! search for field breach
bool CVMapping::searchforPFBreach(const uint16_t* data, const uint16_t len, bool& pfStatus)
{
  bool retVal = false;
  uint8_t wfSeg = 0;
  uint16_t i;
  DPRINTLN(" ");
  for (i = 0; i < len; i++)
  {
    DPRINT(data[i], HEX);
    DPRINT(" ");
    //check for pf breach
    if (false == pfStatus) {
      pfStatus = (data[i] & PLS_PF_MASK) ? true : false;
    }
    //check for warning fielf breach on right side
    if ((false == retVal) && (i < PLS_WF_SEGEMENTS_RIGHT))
    {
      retVal = (data[i] & PLS_WF_MASK) ? true : false;
    }
  }
  return (retVal || pfStatus);
}

//! Calculate and update wall information to Navigation application
void CVMapping::calculateWallInfo(const uint16_t* data, const uint16_t len, bool wfStatus, bool pfStatus)
{
  //set default values for vertical distance, right and left offset
  uint16_t VDistance = PLS_MAX_VERTICAL_DIST;
  sint16_t offsetx0 = data[0] & PLS_DIST_MASK;
  sint16_t offsetx180 = data[PLS_WF_SEGEMENTS_MAX - 1] & PLS_DIST_MASK;
  int16_t wallAngleCordic = 0;
  /*check for pfstatus*/
  if (pfStatus) {
    DPRINTLN("PF Breach halt");
    //return immediately
    //wait till pf field is cleared
    return;
  } else if (wfStatus)
  {
    DPRINTLN("WF BREACH checking");
    sint16_t temp = 0;
    sint16_t tempy = 0;
    uint16_t index = PLS_WF_SEGEMENTS_RIGHT;
    uint8_t angle = 0;
    //find shortest vertical distance
    //between right and left corners of warning field
    for (; index < PLS_WF_SEGEMENTS_LEFT_CORNER; index++)
    {
      angle = (index > 90) ? (180 - index) : (index);
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
    //check if any breach on right side of warning field
    for (int i = 1; i < PLS_WF_SEGEMENTS_RIGHT; i++)
    {
      if (PLS_WF_MASK & data[i]) {
        //find closest breached segment to vechicle
        angle = (i > 90) ? (180 - i) : (i);
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
    if ((offsetx0 - offsetx1) > PLS_RIGHT_OFFSET_TOLERANCE)
    {
      // find wall angle to avoid obstacle
      wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety) << 16, static_cast<sint32_t>(offsetx0 - offsetx1) << 16) >> 8);
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
  } else {
    //if no field breach detected
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

    if (offsetx1 > offsetx0) {
      //if vehicle needs to turn right
      DPRINTLN("need to turn to right");
      if ((offsetx1 - offsetx0) > PLS_RIGHT_OFFSET_TOLERANCE) {
        wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety) << 16, static_cast<sint32_t>(offsetx1 - offsetx0) << 16) >> 8);
        wallAngleCordic = -90 + wallAngleCordic;
      }
    } else {
      //if vechicle need to turn left
      DPRINTLN("need to turn to left");
      if ((offsetx0 - offsetx1) > PLS_RIGHT_OFFSET_TOLERANCE) {
        wallAngleCordic = static_cast<sint16_t>(CordicATan(static_cast<sint32_t>(offsety) << 16, static_cast<sint32_t>(offsetx0 - offsetx1) << 16) >> 8);
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
  //updating navigation app
  m_NAV.setPLSdata(offsetx0, wallAngleCordic, VDistance);
  if (!m_driveState)
  {
    //resume driving if paused
    m_NAV.continueDrive();
    m_driveState = true;
  }
}

//! cordic algorithm to find inverse tangent
sint16_t CVMapping::CordicATan(sint32_t y, sint32_t x)
{
  //cordic algoritm
  //refer algorithm details for more info
  sint32_t result = 0;
  sint32_t temp;
  sint16_t z = 0;
  for (uint8_t i = 0U; i < CORDIC_STAGES; i++)
  {
    temp = x;
    if (y < 0)
    {
      x = x - (y >> i);
      y = y + (temp >> i);
      z = z - atan_cordic_table[i];
    }
    else {
      x = x + (y >> i);
      y = y - (temp >> i);
      z = z + atan_cordic_table[i];
    }
    if (0 == y)
    {
      break;
    }
  }
  return z;
}
