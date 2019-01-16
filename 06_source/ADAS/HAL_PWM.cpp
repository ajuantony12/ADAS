/* --------------------------------------------------------------
	Arduino Library to use 16-bit PWM signals on the Arduino Mega

	Author: Christoph Jurczyk
  -------------------------------------------------------------- */

#include <Arduino.h>
#include "HAL_PWM.h"
#include "ADAS_Debug.h"
#define PWM1 11
#define PWM2 12

CPWMUnit::CPWMUnit(PWMID_e ID):
  m_ID(ID)
{
  //do nothing
}
CPWMUnit::~CPWMUnit() {
  m_ID = PWM1;
}

// function to initialize 16-bit PWM
void CPWMUnit::setupPWM16() {
  pinMode(m_ID, OUTPUT);

  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);

  TCCR1B = (1 << WGM12) | (0 << CS12) | (1 << CS10);

}


// function to write PWM duty cycle
// value from 0 to 65535
void CPWMUnit::analogWrite16(uint8_t pin, uint16_t val)
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

void CPWMUnit::writeMOT(uint16_t n)
{
  analogWrite16(m_ID, 1023-n);
}
