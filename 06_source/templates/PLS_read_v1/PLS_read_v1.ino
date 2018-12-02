//#define DEBUG
#include "debug.h"

#define PLS_STX 0x02 // start byte
#define PLS_ADR 0x80 // address of PLS

// PLS telegrams
#define PLS_MW_TGM 0xB0 //Response to measure value request

// PLS MW_TGM
struct mw_tgm_struct {
  uint16_t amw;
  uint16_t *data;
  uint16_t *dist;
  const String dist_unit = "cm";
  bool *glare_flag; // not smart to use bool because bool -> uint8_t
  bool *wf_v_flag;  // not smart to use bool because bool -> uint8_t
  bool *pf_v_flag;  // not smart to use bool because bool -> uint8_t
};
mw_tgm_struct mw_tgm;




void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);
}




void loop() {
  if (Serial1.available()) {
    uint8_t rxData = Serial1.read();
    bool trnsOkay = ReadRecvBuf(rxData);

    if (trnsOkay) {
      //Serial.println("\n\n########\nPLS data received!");
      process_mw_tgm();
      //print_mw_tgm();
      print_mw_tgm_matlab();
    }

  }
}




// Function to read data from PLS
bool ReadRecvBuf(uint8_t buff)
{
  if (buff == PLS_STX) {
    // expecting address
    waitforSerial1data();
    buff = Serial1.read();

    if (buff == PLS_ADR) {
      // expecting length
      waitforSerial1data();
      buff = Serial1.read();
      waitforSerial1data();
      uint8_t buff_high = Serial1.read();

      // length of message excluding CRC
      uint16_t data_len = (buff_high << 8) + buff;

      // expecting command
      waitforSerial1data();
      buff = Serial1.read();
      switch (buff) {
        case PLS_MW_TGM:
          // expecting number of measurements
          waitforSerial1data();
          buff = Serial1.read();
          waitforSerial1data();
          buff_high = Serial1.read();

          mw_tgm.amw = (buff_high << 8) + buff;
          DPRINT("AMW="); DPRINTLN(mw_tgm.amw);

          // change size of mw_tgm.data according to amw value
          mw_tgm.data = realloc(mw_tgm.data, mw_tgm.amw * sizeof(uint8_t));

          // read data[amw]
          for (uint16_t n = 0; n < mw_tgm.amw; n++) {
            waitforSerial1data();
            buff = Serial1.read();
            waitforSerial1data();
            buff_high = Serial1.read();

            mw_tgm.data[n] = (buff_high << 8) + buff;
            DPRINT("MW["); DPRINT(n + 1); DPRINT("]=");
            DPRINTLN(mw_tgm.data[n]);
          }
          break;
      }
      // expecting status
      waitforSerial1data();
      buff = Serial1.read();

      uint8_t tgm_status = buff;

      DPRINT("Status="); DPRINTLN(tgm_status);

      // expecting CRC
      waitforSerial1data();
      buff = Serial1.read();
      waitforSerial1data();
      buff_high = Serial1.read();

      uint16_t crc_val = (buff_high << 8) + buff;

      DPRINT("CRC="); DPRINTLN(crc_val);

      return true;
    }
  }
  return false;
}

bool process_mw_tgm(void)
{
  mw_tgm.dist = realloc(mw_tgm.dist, mw_tgm.amw * sizeof(uint16_t));
  mw_tgm.glare_flag = realloc(mw_tgm.glare_flag, mw_tgm.amw * sizeof(bool));
  mw_tgm.wf_v_flag = realloc(mw_tgm.wf_v_flag, mw_tgm.amw * sizeof(bool));
  mw_tgm.pf_v_flag = realloc(mw_tgm.pf_v_flag, mw_tgm.amw * sizeof(bool));

  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    mw_tgm.dist[n] = mw_tgm.data[n] & 0x01FFF; // measured distance
    mw_tgm.glare_flag[n] =  mw_tgm.data[n] & 0x2000; // glare flag
    mw_tgm.wf_v_flag[n] =  mw_tgm.data[n] & 0x4000; // WF-V-flag
    mw_tgm.pf_v_flag[n] =  mw_tgm.data[n] & 0x8000; // WF-V-flag
  }

  return true;
}

void print_mw_tgm(void)
{
  char buf[50];

  for (uint16_t n = 0; n < mw_tgm.amw; n++) {
    Serial.print("alpha="); Serial.print(n + 1); Serial.print(" deg:\t");
    Serial.print("raw="); PrintHex16(mw_tgm.data[n], 1); Serial.print("\t\t");
    Serial.print("dist["); Serial.print(mw_tgm.dist_unit); Serial.print("]="); sprintf(buf, "%4d", mw_tgm.dist[n]); Serial.print(buf); Serial.print("\t\t");
    Serial.print("glare="); Serial.print(mw_tgm.glare_flag[n]); Serial.print("\t\t");
    Serial.print("wf_v_flag="); Serial.print(mw_tgm.wf_v_flag[n]); Serial.print("\t");
    Serial.print("pf_v_flag="); Serial.print(mw_tgm.pf_v_flag[n]); Serial.println("");
  }
}

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

void waitforSerial1data(void)
{
  while (!Serial1.available()) {}
}


void PrintHex16(uint16_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
  char tmp[16];
  for (int i = 0; i < length; i++)
  {
    sprintf(tmp, "0x%.4X", data[i]);
    Serial.print(tmp); Serial.print(" ");
  }
}

