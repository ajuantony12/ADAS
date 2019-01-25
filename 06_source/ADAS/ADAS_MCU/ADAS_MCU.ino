#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"

/*Periperals*/
#include "HAL_DriveUnit.h"
#include "HAL_Encoder.h"
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "HAL_Serial_IF.h"

/*Comms layer*/
#include "Comm_Inertial.h"
#include "Comm_ICC.h"

/*App Layer*/
#include "App_MotorCtrl.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"


//Hardware
  CIMUUnit    imu_o;
  CEncoder    enc1_o(CEncoder::E1);
  CEncoder    enc2_o(CEncoder::E2);
  CPWMUnit    pwmUnitLeft_o(CPWMUnit::PWM1);
  CPWMUnit    pwmUnitRight_o(CPWMUnit::PWM2);
  CSerial     iccPort(CSerial::S2, ICC_RCV_BUFF_SIZE);
//comms layer
  CICCComms iccComms_o(iccPort);
  CMotorCtrl  mCtrl_o(imu_o, pwmUnitLeft_o, pwmUnitRight_o, enc1_o, enc2_o, iccComms_o);

void setup() {
  //Hw initialization
  Serial.begin(9600);
  DPRINTLN("Hello\n\r");

  //inertial_o.Init();
  mCtrl_o.Init();
  iccPort.Init();
  iccComms_o.Init(&mCtrl_o);
  interrupts();
}

void loop() {
  iccComms_o.Run();
  mCtrl_o.Run();
}
// UART2 interrupt
ISR(USART2_RX_vect)
{
 iccPort.SerialISRcommICC();
}
