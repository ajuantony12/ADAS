#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"

/*Periperals*/
#include "HAL_IOManager.h"
#include "HAL_Serial_IF.h"

/*Comms layer*/
#include "Comm_ICC.h"

/*App Layer*/
#include "App_Navigation.h"
#include "App_EnvironmentalData.h"

//Hardware
CSerial     plsPort(CSerial::S1, PLS_RCV_BUFF_SIZE);
CSerial     iccPort(CSerial::S2, ICC_RCV_BUFF_SIZE);
CNavigation nav_o;
CEnvironmentalData env_o;
//comms layer
CICCComms iccComms_o(iccPort);

// Debug pins
#define PIN_ROT_DONE    12
#define PIN_DIST_DONE   11


void setup() {
  //Hw initialization
  Serial.begin(115200);
  DPRINTLN("Hello\n\r");
  plsPort.Init();
  iccPort.Init();



  // IO Init
  pinMode(PIN_ROT_DONE, INPUT_PULLUP);
  pinMode(PIN_DIST_DONE, INPUT_PULLUP);


  interrupts();
}

void loop() {
  // Set debug data...
  nav_o.rotationDone = !digitalRead(PIN_ROT_DONE);
  nav_o.distanceDone = !digitalRead(PIN_DIST_DONE);

  /*
    char datain = Serial.read();
    if (datain == 's') {
      uint16_t offset = Serial.parseInt();
      int8_t angle = Serial.parseInt();
      uint16_t nxt_wall = Serial.parseInt();

      nav_o.setPLSdata(offset, angle, nxt_wall);
    } else if (datain == 'd') {
      DPRINTLN("Run");
      nav_o.continueDrive();
    } else if (datain == 'f') {
      DPRINTLN("Stop");
      nav_o.stopDrive();
    }
  */


  // Run ICC
  iccComms_o.Run();
  iccComms_o.addTxMsg(0x01, -200);
  // Run navigation
  //nav_o.printChangedDebugInfo();
  // nav_o.Run();
  // Run env
  //env_o.Run();


  delay(500);
}



// UART1 interrupt
ISR(USART1_RX_vect)
{
  plsPort.SerialISRcommPLS();
}


// UART2 interrupt
ISR(USART2_RX_vect)
{
  iccPort.SerialISRcommICC();
}
