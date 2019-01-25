// Test Program for T-FUNC-009-2
// Author: Christoph Jurczyk
// Manual: -> Press button 
//         -> AV is waiting DELAY_TIME
//         -> AV runs TEST_LEN backward
//         To repeat test just press the button

#define BAUDRATE 9600
#define TEST_LEN 100
#define PIN_START 12
#define DELAY_TIME 2000

bool runTest = false;
bool testFinished = false;

void setup() {
  pinMode(PIN_START, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.begin(BAUDRATE, SERIAL_8E1);
  Serial.println("\nStarted...");
}

void loop() {

  if (!digitalRead(PIN_START) && !runTest)
  {
    Serial.println("Run test!");
    delay(DELAY_TIME);
    sendMsg(0x04, -TEST_LEN);
    runTest = true;
  }


  if (Serial2.available())
  {
    uint8_t mcu_status = parseMsg();

    if ((mcu_status == 0x02) && (testFinished == false))
    {
      Serial.println("Reached target!");
      testFinished = true;

      runTest = false;
    }
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

  Serial.print("Sent command!");
  Serial.print("\t");
  Serial.print("cmd = 0x");
  Serial.print(cmd, HEX);
  Serial.print("\t");
  Serial.print("data = ");
  Serial.println(data);
}


uint8_t parseMsg(void)
{
  uint8_t rxData[6];
  uint8_t n = 0;

  while (n < 6)
  {
    while (!Serial2.available()) {};
    rxData[n] = Serial2.read();
    n++;
  }

  if ((rxData[0] == 0xFF) && (rxData[1] == 0xFF) && (rxData[5] == 0xEE))
  {
    return rxData[2];
  }

  return 0;

}

