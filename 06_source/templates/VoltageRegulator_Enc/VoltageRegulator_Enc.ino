#include "FastPID.h"
#include "PWM_16bit.h"
#include "Timer.h"

#define PIN_ENC 3
#define WAIT 50
#define LED1 11
#define LED2 12
#define PIN_ENABLE 52
#define PIN_DIRECTION 50

boolean control = false;
volatile unsigned long time_uC;
volatile unsigned long enc_last_time = 0;
volatile unsigned long enc_t = 0;
uint16_t rpm = 0;
uint16_t setpoint;
uint16_t feedback;
uint16_t output;
uint16_t peaks = 0;
uint16_t counted_peaks = 0;
Timer t;
float Kp = 0.1, Ki = 0.5, Kd = 0, Hz = 10;
int output_bits = 16;
bool output_signed = false;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);

void setup()
{
  Serial.begin(9600);
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIRECTION, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(PIN_ENC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC), EncISR, RISING);
  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(PIN_DIRECTION, HIGH);
  t.every(500, readenc, 0);
  setupPWM16();
}

void loop()
{
  if (control) {
 
    setpoint = 200;
    feedback = counted_peaks*4;
    output = myPID.step(setpoint, feedback);

    if (output >= 1023) {
      output = 1023;
    }
    Serial.print(output);
    Serial.print(";");
    Serial.println(counted_peaks);
    if (digitalRead(PIN_ENABLE) == HIGH) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    } else {
      writeLED(output);
    }
    control=false;
  }
  t.update();
}


void writeLED(uint16_t n)
{
  analogWrite16(LED1, 1023 - n);
  analogWrite16(LED2, 1023 - n);
}


void EncISR(void)
{
  peaks++;
}

void readenc(void* context) {
  counted_peaks = peaks;
  control = true;
  peaks = 0;
}
