/**
* @file HAL_PWM.cpp
* @author Christoph Jurczyk
* @date January 29, 2019
* @brief Hardware Abstraction Layer (HAL) for the 10-bit PWM outputs
*
*/

#include <Arduino.h>
#include "HAL_PWM.h"
#include "ADAS_Debug.h"
#define PWM1 11
#define PWM2 12

//! Constructor of CPWMUnit
/*!
  \param ID Pin number
*/
CPWMUnit::CPWMUnit(PWMID_e ID):
  m_ID(ID)
{
  //do nothing
}
//! Destructor of CPWMUnit
CPWMUnit::~CPWMUnit() {
  m_ID = PWM1;
}

//! Function to initialize timers for 10-bit PWM
void CPWMUnit::setupPWM10() {
  // Set pin as output
  pinMode(m_ID, OUTPUT);

  // Configure timer registers
  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0);
  TCCR1B = (1 << WGM12) | (0 << CS12) | (1 << CS10);
}


//! Function to write PWM duty cycle to timer overflow value
/*!
  \param pin Pin number
  \param val Duty cycle (0 = 0% and 1023 = 100%)
*/
void CPWMUnit::analogWrite10(uint8_t pin, uint16_t val)
{
  switch (pin) {
    case  11:
      OCR1AH = (val >> 8);
      OCR1AL = val & 0x00ff;
      break;
    case  12:
      OCR1BH = (val >> 8);
      OCR1BL = val & 0x00ff;
      break;
  }
}

//! Function write duty cycle
/*!
  \param n Duty cycle (0 = 0% and 1023 = 100%)
*/
void CPWMUnit::writeDuty(uint16_t n)
{
  analogWrite10(m_ID, n);
}
