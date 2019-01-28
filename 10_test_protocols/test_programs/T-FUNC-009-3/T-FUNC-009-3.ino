// Test Program for T-FUNC-009-3
// Author: Christoph Jurczyk
// Manual: -> Press button
//         -> AV is waiting DELAY_TIME
//         -> AV rotates first angle to right
//         -> AV is waiting until next button press
//         -> AV is waiting DELAY_TIME
//         -> AV rotates first angle to right
//         Repeat for next angles as described above

#define BAUDRATE 9600
#define PIN_START 12
#define DELAY_TIME 2000

int16_t test_angles[6] = {180, 135, 90, 45, 20, 10};
uint8_t curr_tag_angle = 0;

bool runTest = false;
bool testFinished = true;

void setup() {
  pinMode(PIN_START, INPUT_PULLUP);

  Serial.begin(115200);
  Serial2.begin(BAUDRATE, SERIAL_8E1);
  Serial.println("\nStarted...");
}

void loop() {

  if (!digitalRead(PIN_START) && testFinished)
  {
    Serial.println("Run test!");
    delay(DELAY_TIME);

    Serial.print("Rotate angle: ");
    Serial.print(test_angles[curr_tag_angle]);
    Serial.println(" deg");

    sendMsg(0x05, test_angles[curr_tag_angle]);
    testFinished = false;
  }


  if (Serial2.available())
  {
    uint8_t mcu_status = parseMsg();

    if ((mcu_status == 0x03) && (testFinished == false))
    {
      if (curr_tag_angle == sizeof(test_angles))
      {
        curr_tag_angle = 0;
        Serial.println("Test finished!");
      } else {
        curr_tag_angle++;
      }

      testFinished = true;
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

