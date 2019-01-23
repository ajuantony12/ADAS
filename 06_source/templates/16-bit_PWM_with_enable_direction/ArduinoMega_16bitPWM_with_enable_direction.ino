#include "PWM_16bit.h"

#define WAIT 50
#define LED1 11
#define LED2 12
#define PIN_ENABLE 52
#define PIN_DIRECTION 50

void setup() {
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(PIN_ENABLE, LOW); 
  digitalWrite(PIN_DIRECTION, HIGH);
  setupPWM16();
}

void loop() {


  if (digitalRead(PIN_ENABLE)== HIGH) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);   
  }else{
    for (uint16_t n = 0; n < 65535; n++)
    {
      writeLED(n);
    }
    for (uint16_t n = 65535; n > 0; n--)
    {
      writeLED(n);
    }
  }


}

void writeLED(uint16_t n)
{
  analogWrite16(LED1, n);
  analogWrite16(LED2, 65535 - n);
  delayMicroseconds(WAIT);
}
