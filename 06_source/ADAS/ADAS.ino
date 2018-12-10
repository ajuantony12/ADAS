#include <ADAS_Types.h>
#include <ADAS_Cfg.h>
#include <ADAS_Debug.h>

/*Periperals*/
#include <ADC.h>
#include <DriveUnit.h>
#include <Encoder.h>
#include <IOManager.h>
#include <Serial_IF.h>

/*Comms layer*/
#include <InertialComms.h>
#include <PLSComms.h>

/*App Layer*/
#include <MotorCtrl.h>
#include <Navigation.h>
#include <Positioning.h>
#include <UserInterface.h>
#include <VirtualMapping.h>

/*OSAL*/
#include <TaskCtrl.h>

unsigned short len;
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
  DPRINTLN("HEllo\n\r");

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
