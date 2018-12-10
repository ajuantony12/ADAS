#include "PLS_stim_data.h"

#define BAUDRATE 115200
#define BAUDRATE1 9600

bool transmit_data = false;
bool transmit_data_cont = false;
uint16_t transmit_counter = 0;
uint8_t selected_dataset = 1;
uint8_t preselected_dataset = 0;

void setup() {
  Serial.begin(BAUDRATE);
  Serial1.begin(BAUDRATE1, SERIAL_8E1);
  pinMode(LED_BUILTIN, OUTPUT);

  delay(2000);
  Serial.println("PLS Simulator started!");
  Serial.print("Baudrate="); Serial.println(BAUDRATE1);
  printHeader();

}

void loop() {
  checkUSBinput();
  sendPLSdata();
}


void printHeader(void)
{
  Serial.println("(a) for single run (c) continuous run (s) for stop (d + 1/2) for data selection");
}

void checkUSBinput(void)
{
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if ((inChar == 'a') && !transmit_data) {
      Serial.println("Starting transmission single run...");
      transmit_data = true;
      transmit_data_cont = false;
      digitalWrite(LED_BUILTIN, HIGH);
    } else if ((inChar == 'c') && !transmit_data) {
      Serial.println("Starting transmission continuous...");
      transmit_data = true;
      transmit_data_cont = true;
      digitalWrite(LED_BUILTIN, HIGH);
    } else if ((inChar == 's') && transmit_data) {
      Serial.println("Stopping transmission...");
      //printHeader();
      //transmit_data = false;
      transmit_data_cont = false;
      //transmit_counter = 0;
      //digitalWrite(LED_BUILTIN, LOW);
    } else if ((inChar == 'd') && !transmit_data) {
      selected_dataset = Serial.parseInt();
      switch (selected_dataset) {
        case 1:
          Serial.print("Selected dataset: ");
          Serial.println(PLS_stim_data1_name);
          break;
        case 2:
          Serial.print("Selected dataset: ");
          Serial.println(PLS_stim_data2_name);
          break;
        case 3:
          Serial.print("Selected dataset: ");
          Serial.println(PLS_stim_data3_name);
          break;
        default:
          Serial.print("Selected dataset is not available! Switched to default.");
          selected_dataset = 1;
          break;
      }
    } else if ((inChar == 'd') && transmit_data) {
      preselected_dataset = Serial.parseInt();
      switch (preselected_dataset) {
        case 1:
          Serial.print("Preselected dataset: ");
          Serial.println(PLS_stim_data1_name);
          break;
        case 2:
          Serial.print("Preselected dataset: ");
          Serial.println(PLS_stim_data2_name);
          break;
        case 3:
          Serial.print("Preselected dataset: ");
          Serial.println(PLS_stim_data3_name);
          break;
        default:
          Serial.print("Preselected dataset is not available!");
          preselected_dataset = 0;
          break;
      }
    }
  }
}

void sendPLSdata(void)
{
  if (transmit_data)
  {
    switch (selected_dataset) {
      case 1:
        Serial.println(PLS_stim_data1[transmit_counter], HEX);
        Serial1.write(PLS_stim_data1[transmit_counter]);
        break;
      case 2:
        Serial.println(PLS_stim_data2[transmit_counter], HEX);
        Serial1.write(PLS_stim_data2[transmit_counter]);
        break;
      case 3:
        Serial.println(PLS_stim_data3[transmit_counter], HEX);
        Serial1.write(PLS_stim_data3[transmit_counter]);
        break;
      default:
        Serial.println("Error!");
        break;
    }
    // delay between telegrams
    if ((transmit_counter == (PLS_stim_len / 2) - 1) || (transmit_counter == PLS_stim_len -1))
    {
      Serial.println("Next telegram");
      delay(300);
    }
    transmit_counter++;
    if (transmit_counter >= PLS_stim_len) {
      Serial.println("End of simulation data...");
      if (!transmit_data_cont) {
        transmit_data = false;
        digitalWrite(LED_BUILTIN, LOW);
        printHeader();
      } else if (preselected_dataset > 0) {
        Serial.println("Switching dataset!");
        selected_dataset = preselected_dataset;
        preselected_dataset = 0;
      }
      transmit_counter = 0;
    }
  }
}

