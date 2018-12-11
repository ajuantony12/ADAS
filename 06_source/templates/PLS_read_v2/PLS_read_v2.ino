#define DEBUG
#include "debug.h"

const byte rxPin = 19;
uint16_t counter = 0;
bool rxINT = true;

byte rxBuffer[370];


#define PLS_STX 0x02 // start byte
#define PLS_ADR 0x80 // address of PLS

// PLS telegrams
#define PLS_MW_TGM 0xB0 //Response to measure value request

// PLS MW_TGM
struct mw_tgm_struct {
  uint16_t amw;
  uint16_t dist[180];
  uint16_t data[180];
  uint8_t tgm_status;
  const String dist_unit = "cm";
  bool glare_flag[180]; // not smart to use bool because bool -> uint8_t
  bool wf_v_flag[180];  // not smart to use bool because bool -> uint8_t
  bool pf_v_flag[180];  // not smart to use bool because bool -> uint8_t
};
mw_tgm_struct mw_tgm;

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8E1);
  attachInterrupt(digitalPinToInterrupt(rxPin), serial_read, HIGH);
}




void loop() {
  if (counter == 369) // received full message without last CRC byte
  {
    rxINT = false;
    counter = 0;
    bool prcsOk = ReadRecvBuf(rxBuffer);

    if (prcsOk) {
      process_mw_tgm();
      // print_mw_tgm();
      //print_mw_tgm_matlab_cmd();
      print_mw_tgm_matlab();
    }

    // enable interrupt for new data
    rxINT = true;
  }
}


void serial_read(void)
{
  if (rxINT)
  {
    // Disable interrupts
    sei();
    while (Serial1.available() > 0) {
      rxBuffer[counter] = Serial1.read();
      counter++;

      // Check for STX & ADR
      if (counter == 3)
      {
        if ((rxBuffer[0] != PLS_STX) && (rxBuffer[1] != PLS_ADR))
        {
          // STX & ADR not in first two bytes => overwrite received data
          counter = 0;
        }
      }

    }
    // enable interrupts
    cli();
  }
}



// Function to read data from PLS
bool ReadRecvBuf(byte *rxBuffer) {

  // read length of tgm excluding CRC
  uint16_t data_len = (rxBuffer[3] << 8) + rxBuffer[2];
  DPRINT("data_len="); DPRINTLN(data_len, HEX);

  // read command
  switch (rxBuffer[4])
  {
    case PLS_MW_TGM:
      // read expecting number of measurements
      mw_tgm.amw = (rxBuffer[6] << 8) + rxBuffer[5];
      DPRINT("AMW="); DPRINTLN(mw_tgm.amw);

      // change size of mw_tgm.data according to amw value
      // To enable change in struct array to * without any length
      //mw_tgm.data = realloc(mw_tgm.data, mw_tgm.amw * sizeof(uint8_t));

      // read data[amw]
      for (uint16_t n = 0; n < mw_tgm.amw; n++) {
        mw_tgm.data[n] = (rxBuffer[n * 2 + 10] << 8) + rxBuffer[n * 2 + 9];
        DPRINT("MW["); DPRINT(n + 1); DPRINT("]=");
        DPRINTLN(mw_tgm.data[n]);
      }

      // read status
      mw_tgm.tgm_status = rxBuffer[mw_tgm.amw * 2 + 7];
      DPRINT("mw_tgm.tgm_status="); DPRINTLN(mw_tgm.tgm_status, HEX);

      // finish progressing
      return true;

      break;

    default:
      break;
  }

  return false;
}



// function to progress received mw_tgm
bool process_mw_tgm(void)
{
  /*mw_tgm.dist = realloc(mw_tgm.dist, mw_tgm.amw * sizeof(uint16_t));
    mw_tgm.glare_flag = realloc(mw_tgm.glare_flag, mw_tgm.amw * sizeof(bool));
    mw_tgm.wf_v_flag = realloc(mw_tgm.wf_v_flag, mw_tgm.amw * sizeof(bool));
    mw_tgm.pf_v_flag = realloc(mw_tgm.pf_v_flag, mw_tgm.amw * sizeof(bool));*/

  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    mw_tgm.dist[n] = mw_tgm.data[n] & 0x01FFF; // measured distance
    mw_tgm.glare_flag[n] =  mw_tgm.data[n] & 0x2000; // glare flag
    mw_tgm.wf_v_flag[n] =  mw_tgm.data[n] & 0x4000; // WF-V-flag
    mw_tgm.pf_v_flag[n] =  mw_tgm.data[n] & 0x8000; // WF-V-flag
  }

  return true;
}



// function to print mw_tgm via Serial
void print_mw_tgm(void)
{
  char buf[50];
  Serial.println("\n\n########\nPLS data received!");

  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print("alpha="); Serial.print(n + 1); Serial.print(" deg:\t");
    Serial.print("raw="); Serial.print(mw_tgm.data[n], HEX); Serial.print("\t");
    Serial.print("dist["); Serial.print(mw_tgm.dist_unit); Serial.print("]="); sprintf(buf, "%4d", mw_tgm.dist[n]); Serial.print(buf); Serial.print("\t\t");
    Serial.print("glare="); Serial.print(mw_tgm.glare_flag[n]); Serial.print("\t\t");
    Serial.print("wf_v_flag="); Serial.print(mw_tgm.wf_v_flag[n]); Serial.print("\t");
    Serial.print("pf_v_flag="); Serial.print(mw_tgm.pf_v_flag[n]); Serial.println("");
  }
}


// function to print manual command for matlab
void print_mw_tgm_matlab_cmd(void)
{
  Serial.println("Run in matlab:");
  Serial.println("angle = linspace(0,pi,180);");
  Serial.print("pls_dist = [");
  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print(mw_tgm.dist[n]); Serial.print(", ");
  }
  Serial.println("];");
  Serial.println("polarplot(angle,pls_dist)");
}


// function to print data string fro matlab gui
void print_mw_tgm_matlab(void)
{
  Serial.print("1;");
  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print(mw_tgm.dist[n]); Serial.print(";");
  }
  Serial.println("");


  Serial.print("2;");
  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print(mw_tgm.glare_flag[n]); Serial.print(";");
  }
  Serial.println("");


  Serial.print("3;");
  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print(mw_tgm.wf_v_flag[n]); Serial.print(";");
  }
  Serial.println("");


  Serial.print("4;");
  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print(mw_tgm.pf_v_flag[n]); Serial.print(";");
  }
  Serial.println("");


}
