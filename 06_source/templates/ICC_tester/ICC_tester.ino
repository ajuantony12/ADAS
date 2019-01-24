#define BAUDRATE 9600

void setup() {
  Serial.begin(115200);
  Serial2.begin(BAUDRATE, SERIAL_8E1);
  Serial.println("\nStarted...");

}

void loop() {

  char serial_input = Serial.read();
  //a[cmd];[data]
  if (serial_input == 'a')
  {
    uint8_t cmd = Serial.parseInt();
    int16_t data = Serial.parseInt();

    sendMsg(cmd, data);
  }


  while (Serial2.available())
  {
    byte serial2_input = Serial2.read();
    Serial.print("Rx data: 0x");
    Serial.println(serial2_input, HEX);
  }


}


void sendMsg(uint8_t cmd, int16_t data)
{

  Serial2.write(0xFF);
  Serial2.write(0xFF);
  Serial2.write(cmd);
  Serial2.write((uint8_t)(data >> 8));
  Serial2.write((uint8_t)(data & 0x00FF));
  Serial2.write(0xEE);

  Serial.print("cmd = 0x");
  Serial.println(cmd, HEX);
  Serial.print("data = ");
  Serial.println(data);

  Serial.println("Sent!");
}

