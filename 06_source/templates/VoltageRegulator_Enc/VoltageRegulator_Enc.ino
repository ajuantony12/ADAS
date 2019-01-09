#include "FastPID.h"
#include "PWM_16bit.h"
#include "Timer.h"

#define PIN_ENC_L 2
#define PIN_ENC_R 3
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
int n = 0;
uint16_t setpoint = 400;
uint16_t feedback;
uint16_t output_r = 0;
uint16_t output_l = 0;
uint16_t peaks_r = 0;
uint16_t peaks_l = 0;
uint16_t counted_peaks_r = 0;
uint16_t counted_peaks_l = 0;
uint16_t peak_sum_l = 0;
uint16_t peak_sum_r = 0;
uint16_t d_way = 2228; // desired way to drive in cm (1.795cm/peak) 2228==4000cm
Timer t;
float Kp = 0.2, Ki = 0.5, Kd = 0, Hz = 10;
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
  pinMode(PIN_ENC_R, INPUT_PULLUP);
  pinMode(PIN_ENC_L, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), EncISR_R, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), EncISR_L, RISING);
  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(PIN_DIRECTION, HIGH);
  t.every(500, readenc, 0);
  setupPWM16();
}

void loop()
{
  if (control && peak_sum_l < d_way && peak_sum_r < d_way){
 //right PI control
    feedback = counted_peaks_r*4;
    output_r = myPID.step(setpoint, feedback);

    if (output_r >= 1023) {
      output_r = 1023;
    }
    
//Debug output
    Serial.print(output_r);
    Serial.print(";");
    Serial.print(counted_peaks_r);
    Serial.print(";");
    Serial.print(" Peaks: ");
    Serial.print(peak_sum_r);
   Serial.print("; ");
    
//left PI control

    feedback = counted_peaks_l*4;
    output_l = myPID.step(setpoint, feedback);

    if (output_l >= 1023) {
      output_l = 1023;
    }

//Debug output
   Serial.print(output_l);
   Serial.print(";");
    Serial.print(counted_peaks_l);
    Serial.print(";");
    Serial.print(" Peaks: ");
    Serial.print(peak_sum_l);
   Serial.println("; ");

//write to motor
    if (digitalRead(PIN_ENABLE) == HIGH) {
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, LOW);
    } else {
      writeLED(output_l, output_r);
    }
    control=false;
  }else if(peak_sum_l >= d_way && peak_sum_r >= d_way){
    writeLED(LOW, LOW);
    }
  t.update();
}


void writeLED(uint16_t n1, uint16_t n2)
{
  analogWrite16(LED1, 1023 - n1);
  analogWrite16(LED2, 1023 - n2);
}


void EncISR_L(void)
{
  peaks_l++;
}

void EncISR_R(void)
{
  peaks_r++;
}

void readenc(void* context) {
  counted_peaks_r = peaks_r;
  counted_peaks_l = peaks_l;
  control = true;
  peak_sum_l = peak_sum_l + counted_peaks_l;
  peak_sum_r = peak_sum_r + counted_peaks_r;
  peaks_r = 0;
  peaks_l = 0;
}
