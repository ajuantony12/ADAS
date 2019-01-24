#include "PWM_16bit.h"

#define WAIT 10
#define LED1 11
#define LED2 12

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  setupPWM16();
}

void loop() {
  for (uint16_t n = 0; n < 65535; n++)
  {
    writeLED(n);
  }
  for (uint16_t n = 65535; n > 0; n--)
  {
    writeLED(n);
  }


}

void writeLED(uint16_t n)
{
  analogWrite16(LED1, n);
  analogWrite16(LED2, 65535 - n);
  delayMicroseconds(WAIT);
}
