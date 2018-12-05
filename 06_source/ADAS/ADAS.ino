#include <ADAS_Types.h>
#include <ADAS_Cfg.h>

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
  Serial.begin(9600);
  serPort.Init(SERIAL1_INITIAL_BAUD_RATE, SERIAL1_TIMEOUT);
  Serial.write("HEllo\n\r");

  //inertial_o.Init();
  //plsCOmms_o.Init();         

  //Task initialization
  /*taskCtrl_o.Register(&mCtrl_o, 1);
  taskCtrl_o.Register(&nav_o, 0);
  taskCtrl_o.Register(&pos_o, 2);*/
  //taskCtrl_o.Register(&vMap_o, 3); 
  //taskCtrl_o.Register(&uI_o, 4);
 //taskCtrl_o.Init();
  Serial.write("Hello Init ended\n\r");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.write("In loop\n\r");
  //taskCtrl_o.Run();
  delay(2000);
}

void serialEvent1(){
  char data[5]= {0};
  uint8_t byte1;
  Serial.write("in Event\n\r");
  while (Serial1.available())
  {
    Serial1.readBytes(&byte1,1);
    sprintf(data , "%x",byte1);
    Serial.write(data);
  }
}
