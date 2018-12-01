#include "PLS_stim_data.h"

#define BAUDRATE 115200
#define BAUDRATE1 9600

bool transmit_data = false;
uint16_t transmit_counter = 0;

void setup() {
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE1);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(2000);
  printHeader();

}

void loop() {
  checkUSBinput();
  sendPLSdata();
}


void printHeader(void)
{
  Serial.println("PLS Simulator started!");
  Serial.print("Baudrate="); Serial.println(BAUDRATE1);
  Serial.println("(a) for start (b) for stop");
}

void checkUSBinput(void)
{
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar == 'a') && !transmit_data) {
      Serial.println("Starting transmission...");
      transmit_data = true;
      digitalWrite(LED_BUILTIN, HIGH);
    } else if ((inChar == 'b') && transmit_data) {
      Serial.println("Stopping transmission...");
      Serial.println("(a) for start (b) for stop");
      transmit_data = false;
      transmit_counter = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void sendPLSdata(void)
{
  if (transmit_data)
  {
    Serial.println(PLS_stim_data[transmit_counter], HEX);
    Serial1.write(PLS_stim_data[transmit_counter]);
    transmit_counter++;
    if (transmit_counter >= PLS_stim_len) {
      Serial.println("End of simulation data...");
      transmit_data = false;
      transmit_counter = 0;
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

