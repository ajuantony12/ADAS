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

uint16_t len;
CPLSComms::Message_t msg;


//Hardware
/*CADC adc_o;
  CDriveUnit  dUnitLeft_o(CDriveUnit::Drive1);
  CDriveUnit  dUnitRight_o(CDriveUnit::Drive2);
  CEncoder    enc1_o(CEncoder::E1);
  CEncoder    enc2_o(CEncoder::E2);
  CIOManager  ioMg_o;*/
CSerial     serPort(CSerial::Port1);
//comms layer
//CInertialComm inertial_o;
CPLSComms   plsCOmms_o(serPort);

//Task
CVMapping vMap_o(plsCOmms_o);
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
  serPort.Init(SERIAL1_INITIAL_BAUD_RATE, SERIAL1_TIMEOUT);
  Serial.begin(9600);

  //inertial_o.Init();
  plsCOmms_o.Init();

  //Task initialization
  /*taskCtrl_o.Register(&mCtrl_o, 1);
    taskCtrl_o.Register(&nav_o, 0);
    taskCtrl_o.Register(&pos_o, 2);*/
  taskCtrl_o.Register(&vMap_o, 3);
  //taskCtrl_o.Register(&uI_o, 4);
  //taskCtrl_o.Init();
}

void loop() {
   uint16_t len = 0;
   uint8_t* buff;
  // put your main code here, to run repeatedly:
  //taskCtrl_o.Run();
  DPRINTLN("Sending measurement request");
  if (plsCOmms_o.GetMeasurements(buff, len))
  {
    DPRINTLN("recieved measurement");
    DPRINTLN(len, HEX);
  }
  if (plsCOmms_o.isContaminated())
  {
    DPRINTLN("Warning Field Breached");
  }
  delay(3000);

}

void serialEvent1() {
  while (serPort.Available())
  {
    //      DPRINTLN("DATA available");
    //      if (CPLSComms::MsgSuccess == plsCOmms_o.RecievePkt(len))
    //      {
    //        DPRINTLN("Searching");
    //        plsCOmms_o.SearchMsg(msg, 0x00, len);
    //      }
    plsCOmms_o.AddContaminationAlert();
    DPRINTLN("Warning Field Breached data recieved");
    while (serPort.Available())
      Serial1.read();
  }

}
