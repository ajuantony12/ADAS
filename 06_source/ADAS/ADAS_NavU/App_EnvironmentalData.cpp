#include "App_EnvironmentalData.h"
#include <Arduino.h>

CEnvironmentalData::CEnvironmentalData()
{
  //do nothing
}
CEnvironmentalData::~CEnvironmentalData() {
  //do nothing
}

void CEnvironmentalData::Init(void)
{

}

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
    DPRINTLN("Battery voltage critical! Please charge immediately!");
  }else if((vbat >= ENV_VBAT_CRI) && (vbat <= ENV_VBAT_LOW))
  {
    DPRINTLN("Battery voltage low! Please charge after operation!");
  }
}

void CEnvironmentalData::Stop(void)
{
  //do nothing
}

uint16_t CEnvironmentalData::GetVbat(void)
{
  // Read ADC value and convert to mV
  return m_ADC.ReadADCValue(PIN_VBAT) * ENV_VBAT_GAIN + ENV_VBAT_OFF;

}