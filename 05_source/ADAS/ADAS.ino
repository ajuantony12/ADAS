#include <Serial_IF.h>

char str1[5] = "Hello";
CSerialIF serialPort(CSerialIF::Port1);

void setup() {
  // put your setup code here, to run once:
  serialPort.Init(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  Serial.write(str1, 5);
}
