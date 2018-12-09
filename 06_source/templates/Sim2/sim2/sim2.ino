#define DEBUG
#include "debug.h"

uint8_t** map;
void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600, SERIAL_8E1);

  map = new int[400][400];
}

void loop() {
  // put your main code here, to run repeatedly:

}
