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
#define Btn_Start 46

unsigned long time;
boolean control = false;
volatile unsigned long time_uC;
volatile unsigned long enc_last_time_l = 0;
volatile unsigned long enc_t_l = 166;
volatile unsigned long enc_last_time_r = 0;
volatile unsigned long enc_t_r = 166;
uint16_t rpm = 0;
int n = 0;
uint16_t setpoint = 166-45; 
uint16_t feedback_l;
uint16_t feedback_r;
uint16_t output_r = 0;
uint16_t output_l = 0;
uint16_t peaks_r = 0;
uint16_t peaks_l = 0;
uint16_t counted_peaks_r = 0;
uint16_t counted_peaks_l = 0;
uint16_t peak_sum_l = 0;
uint16_t peak_sum_r = 0;
uint16_t d_way = 1500; // desired way to drive in cm (1.795cm/peak) 2228==4000cm
boolean startBtn = false;
Timer t;
float Kp = 0.5, Ki = 0.5, Kd = 0.05, Hz = 10;
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
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  setupPWM16();
}

void loop()
{
   time = millis();
  //writeLED(100, 100);
  
   //right PI control
      if(enc_t_r > 166){
          feedback_r = 0;
        }else{
            feedback_r = 166-enc_t_r;
          }

      if(enc_t_l > 166){
          feedback_l = 0;
        }else{
            feedback_l = 166-enc_t_l;
          }
      output_l = myPID.step(setpoint, feedback_l);
      output_r = myPID.step(setpoint, feedback_r);
      
  
      if (output_r >= 500) {
        output_r =500;
      }
      if (output_l >= 500) {
        output_l = 500;
      }
      
  //Debug output
      Serial.print(output_r);
      Serial.print(";");
      Serial.print(" Time between r: ");
      Serial.print(enc_t_r);
      Serial.print("; ");
      Serial.print(feedback_r);
      Serial.print(";");
      
  //left PI control
  
  //Debug output
      Serial.print(output_l);
      Serial.print(";");
      Serial.print(" Time between l: ");
      Serial.print(enc_t_l);
      Serial.print("; ");
      Serial.print(feedback_l);
      Serial.println(";");
  
  //write to motor
      if (digitalRead(PIN_ENABLE) == HIGH) {
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
      } else {
        writeLED(output_r*2, output_l*2);
      }
}


void writeLED(uint16_t n1, uint16_t n2)
{
  analogWrite16(LED1, 1023 - n1);
  analogWrite16(LED2, 1023 - n2);
}


void EncISR_L(void)
{
  enc_t_l = millis() - enc_last_time_l;
  enc_last_time_l = millis();
}

void EncISR_R(void)
{
  enc_t_r = millis() - enc_last_time_r;
  enc_last_time_r = millis();
}
