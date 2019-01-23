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
    writeLED(120);



}

void writeLED(uint16_t n)
{
  analogWrite(LED1, n);
  //analogWrite16(LED2, 65535 - n);
  delayMicroseconds(WAIT);
}
