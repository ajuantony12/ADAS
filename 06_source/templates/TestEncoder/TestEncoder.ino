#include "PWM_16bit.h"

#define PIN_ENC 3
#define LED1 11
#define LED2 12
#define PIN_ENABLE 52
#define PIN_DIRECTION 50

unsigned long time;
unsigned long enc_last_time = 0;
unsigned long enc_t = 0;
uint16_t rpm = 0;
int i;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_ENC, INPUT_PULLUP);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC), EncISR, RISING);
  digitalWrite(PIN_ENABLE, LOW); 
  digitalWrite(PIN_DIRECTION, HIGH);
  setupPWM16();
}

void loop() {
  for(i=0; i<=1023; i++){
      writeLED(i);
      Serial.print(enc_t);
      Serial.print(";");
      Serial.println(i);
      delay(100);
    };
    for(i=1023; i>0; i--){
      writeLED(i);
      Serial.print(enc_t);
      Serial.print(";");
      Serial.println(i);
      delay(100);
    };
}

void writeLED(uint16_t n)
{
  analogWrite16(LED1, 1023 - n);
  analogWrite16(LED2, 1023 - n);
}

void EncISR(void)
{
  enc_t = micros() - enc_last_time;
  enc_last_time = micros();
}
