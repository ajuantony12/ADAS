#include "HAL_Encoder.h"
#include "Arduino.h"
#include "ADAS_Debug.h"
#define PIN_ENC 3

unsigned long time;
unsigned long enc_last_time = 0;
unsigned long enc_t = 0;
uint16_t rpm = 0;
  
CEncoder::CEncoder(EncoderID_e ID):
  m_ID(ID)
{
  //do nothing
}
CEncoder::~CEncoder(){
  m_ID = E1;
}

void CEncoder::Init(){
	pinMode(PIN_ENC, INPUT_PULLUP);
  //attachInterrupt(digitalPinToInterrupt(PIN_ENC), EncISR, RISING);
}

unsigned int Read()
{
	   time = micros();
     rpm = 1.0/(70.0*enc_t)*1e6;
     DPRINT("rpm = ");
     DPRINTLN(1.0/(70.0*enc_t)*1e6); // in 1/min
     DPRINT(" enc_t = ");
     DPRINTLN(enc_t); // time between two teeth in microseconds

     return enc_t;
}

bool reset()
{
	//to do 
	return true;
}

void CEncoder::EncISR(void)
{
  enc_t = time - enc_last_time;
  enc_last_time = time;
}
