#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"

/*Periperals*/
#include "HAL_ADC.h"
#include "HAL_DriveUnit.h"
#include "HAL_Encoder.h"
#include "HAL_IOManager.h"
#include "HAL_Serial_IF.h"

/*Comms layer*/
#include "Comm_Inertial.h"
#include "Comm_PLS.h"

/*App Layer*/
#include "App_MotorCtrl.h"
#include "App_Navigation.h"
#include "App_Positioning.h"
#include "App_UserInterface.h"
#include "App_VirtualMapping.h"

/*OSAL*/
#include "OSAL_TaskCtrl.h"

//Hardware
/*CADC adc_o;
  CDriveUnit  dUnitLeft_o(CDriveUnit::Drive1);
  CDriveUnit  dUnitRight_o(CDriveUnit::Drive2);
  CEncoder    enc1_o(CEncoder::E1);
  CEncoder    enc2_o(CEncoder::E2);
  CIOManager  ioMg_o;*/
CSerial     plsPort(CSerial::S1, PLS_RCV_BUFF_SIZE);
CSerial     IPCPort(CSerial::S2, IPC_RCV_BUFF_SIZE);
//comms layer
//CInertialComm inertial_o;
CPLSComms   plsCOmms_o(plsPort);

//Task
CNavigation nav_o(plsCOmms_o);
CVMapping vMap_o(nav_o, plsCOmms_o, 10);
/*CUser_IF uI_o;
  CPositioning pos_o;
  CMotorCtrl mCtrl_o;*/

/*OSAL*/
CTaskCtrl taskCtrl_o;

void setup() {
    Serial.begin(9600);
    Serial.write("hello\n\r");
  //Hw initialization
  /*ioMg_o.Init();
    dUnitLeft_o.Init();
    dUnitRight_o.Init();
    enc1_o.Init();
    enc2_o.Init();*/
  plsPort.Init();
  IPCPort.Init();

  //inertial_o.Init();
   plsCOmms_o.Init();

  //Task initialization
  taskCtrl_o.Register(&vMap_o, 0);
  taskCtrl_o.Register(&nav_o, 1);
  /*taskCtrl_o.Register(&pos_o, 2);
  taskCtrl_o.Register(&uI_o, 4);*/
  taskCtrl_o.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("in Loop\n\r");
  taskCtrl_o.Run();
  delay(1000);

}

// UART1 interrupt
ISR(USART1_RX_vect)
{
  plsPort.SerialISR();
}
// UART1 interrupt
ISR(USART2_RX_vect)
{
  IPCPort.SerialISR();
}
