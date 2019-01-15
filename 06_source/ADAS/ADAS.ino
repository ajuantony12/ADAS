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


uint8_t PLS_RxBuffer[PLS_RCV_BUFF_SIZE] = {0U};
uint8_t IPC_RxBuffer[IPC_RCV_BUFF_SIZE] = {0U};

//Hardware
/*CADC adc_o;
  CDriveUnit  dUnitLeft_o(CDriveUnit::Drive1);
  CDriveUnit  dUnitRight_o(CDriveUnit::Drive2);
  CEncoder    enc1_o(CEncoder::E1);
  CEncoder    enc2_o(CEncoder::E2);
  CIOManager  ioMg_o;*/
CSerial     plsPort(PLS_RxBuffer, CSerial::S1, PLS_RCV_BUFF_SIZE);
CSerial     IPCPort(IPC_RxBuffer, CSerial::S1, IPC_RCV_BUFF_SIZE);
//comms layer
//CInertialComm inertial_o;
CPLSComms   plsCOmms_o(plsPort, PLS_RxBuffer);

//Task
CVMapping vMap_o(plsCOmms_o, 2);
/*CUser_IF uI_o;
  CPositioning pos_o;
  CNavigation nav_o(plsCOmms_o);
  CMotorCtrl mCtrl_o;*/

/*OSAL*/
CTaskCtrl taskCtrl_o;

void setup() {
  //Hw initialization
  /*ioMg_o.Init();
    dUnitLeft_o.Init();
    dUnitRight_o.Init();
    enc1_o.Init();
    enc2_o.Init();*/
  plsPort.Init();
  IPCPort.Init();
  Serial.begin(9600);

  //inertial_o.Init();
  plsCOmms_o.Init();

  //Task initialization
  /*taskCtrl_o.Register(&mCtrl_o, 1);
    taskCtrl_o.Register(&nav_o, 0);
    taskCtrl_o.Register(&pos_o, 2);*/
  taskCtrl_o.Register(&vMap_o, 3);
  /*taskCtrl_o.Register(&uI_o, 4);
    //taskCtrl_o.Init();*/
}

void loop() {
  //Serial1.write("hello\n\r");
  // put your main code here, to run repeatedly:
  //taskCtrl_o.Run();
  if (plsCOmms_o.isContaminated())
  {
    DPRINTLN("Warning Field Breached");
  }
  delay(100);

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
