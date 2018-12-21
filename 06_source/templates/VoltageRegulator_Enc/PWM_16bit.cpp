/* --------------------------------------------------------------
	Arduino Library to use 16-bit PWM signals on the Arduino Mega
	
	Author: Christoph Jurczyk
 -------------------------------------------------------------- */

#include "Arduino.h"
#include "PWM_16bit.h"

// function to initialize 16-bit PWM
void setupPWM16() {

  TCCR1A = (1 << WGM11) | (1 << WGM10) | (1 << COM1A1) | (1 << COM1B1);

  TCCR1B = (1 << WGM12) | (0 << CS12) | (1 << CS10);
                 /* TOP counter value */
}


// function to write PWM duty cycle
// value from 0 to 65535
void analogWrite16(uint8_t pin, uint16_t val)
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
