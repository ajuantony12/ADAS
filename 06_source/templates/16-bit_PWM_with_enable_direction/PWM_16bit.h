/* --------------------------------------------------------------
	Arduino Library to use 16-bit PWM signals on the Arduino Mega
	
	Author: Christoph Jurczyk
 -------------------------------------------------------------- */

#ifndef PWM_16bit_H
#define PWM_16bit_H

#include "Arduino.h"

// function to initialize 16-bit PWM
void setupPWM16();

// function to write PWM duty cycle
// value from 0 to 65535
void analogWrite16(uint8_t pin, uint16_t val);


#endif
