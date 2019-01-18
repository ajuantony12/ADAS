#define PIN_ENC 3

unsigned long time;
unsigned long enc_last_time = 0;
unsigned long enc_t = 0;
uint16_t rpm = 0;


void setup() {
  Serial.begin(9600);
  pinMode(PIN_ENC, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC), EncISR, RISING);
}

void loop() {
  time = micros();

  Serial.print("rpm = ");
  Serial.print(1.0/(70.0*enc_t)*1e6); // in 1/min
  Serial.print(" enc_t = ");
  Serial.println(enc_t); // time between two teeth in microseconds

}


void EncISR(void)
{
  enc_t = time - enc_last_time;
  enc_last_time = time;
}
