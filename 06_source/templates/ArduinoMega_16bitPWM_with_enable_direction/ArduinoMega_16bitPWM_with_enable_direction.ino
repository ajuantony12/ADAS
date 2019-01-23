#include "PWM_16bit.h"

#define WAIT 50
#define LED1 11
#define LED2 12
#define PIN_ENABLE 52
#define PIN_DIRECTION 50
#define PIN_DIRECTION2 48

void setup() {
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(PIN_DIRECTION2, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  digitalWrite(PIN_ENABLE, LOW); 
  digitalWrite(PIN_DIRECTION, HIGH);
  digitalWrite(PIN_DIRECTION2, HIGH);
  setupPWM16();
}

void loop() {


  if (digitalRead(PIN_ENABLE)== HIGH) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);   
  }else{
    for (uint16_t n = 0; n < 35535; n++)
    {
      digitalWrite(PIN_DIRECTION, LOW);
      digitalWrite(PIN_DIRECTION2, HIGH);
      writeLED(n, n);
    }
    for (uint16_t n = 35535; n > 0; n--)
    {
      digitalWrite(PIN_DIRECTION, HIGH);
      digitalWrite(PIN_DIRECTION2, LOW);
      writeLED(n, n);
    }
  }


}

void writeLED(uint16_t n, uint16_t n1)
{
  analogWrite16(LED1, n);
  analogWrite16(LED2, n1);
  delayMicroseconds(WAIT);
}
