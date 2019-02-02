/**
* @file App_EnvironmentalData.cpp
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief Application file for environmental data
*
*/

#include "App_EnvironmentalData.h"
#include <Arduino.h>

//! Constructor of CEnvironmentalData
CEnvironmentalData::CEnvironmentalData()
{
  //do nothing
}
//! Destructor of CEnvironmentalData
CEnvironmentalData::~CEnvironmentalData() {
  //do nothing
}

//! Initialization function of CEnvironmentalData
void CEnvironmentalData::Init(void)
{

}

//! Run function of CEnvironmentalData which is periodically called by CTaskCtrl
/*!
	Function reads battery voltage and prints warning message on serial interface
*/
void CEnvironmentalData::Run(void)
{
  // Read battery voltage
  uint16_t vbat = GetVbat();
  DPRINT("Battery voltate: ");
  DPRINT(vbat);
  DPRINTLN(" mV");

  // Check voltage
  if(vbat < ENV_VBAT_CRI)
  {
    Serial.println("Battery voltage critical! Please charge immediately!");
  }else if((vbat >= ENV_VBAT_CRI) && (vbat <= ENV_VBAT_LOW))
  {
    Serial.println("Battery voltage low! Please charge after operation!");
  }
}

//! Stop function of CEnvironmentalData
void CEnvironmentalData::Stop(void)
{
  //do nothing
}

//! Function to convert ADC value into mV
/*!
  The analog input pin has to defined at ::PIN_VBAT.
  \return Returns the battery voltage in mV according to the parameters ::ENV_VBAT_GAIN and ::ENV_VBAT_OFF
*/
uint16_t CEnvironmentalData::GetVbat(void)
{
  // Read ADC value and convert to mV
  return m_ADC.ReadADCValue(PIN_VBAT) * ENV_VBAT_GAIN + ENV_VBAT_OFF;
}