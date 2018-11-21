#include <ADC.h>
#include <DriveUnit.h>
#include <Encoder.h>
#include <I2C.h>
#include <IOManager.h>
#include <Serial_IF.h>

#include <MotorCtrl.h>
#include <Navigation.h>
#include <Positioning.h>
#include <UserInterface.h>
#include <VirtualMapping.h>
#include <TaskCtrl.h>

//Hardware
CADC adc_o;
CDriveUnit  dUnitLeft_o(CDriveUnit::Drive1);
CDriveUnit  dUnitRight_o(CDriveUnit::Drive2);
CEncoder    enc1_o(CEncoder::E1);
CEncoder    enc2_o(CEncoder::E2);
CI2C        i2c_o(CI2C::I2C1);          
CIOManager  ioMg_o;
CSerial     serPort(CSerial::Port1);

//Task
CTaskCtrl taskCtrl_o;
CVMapping vMap_o;
CUser_IF uI_o;
CPositioning pos_o;
CNavigation nav_o;
CMotorCtrl mCtrl_o;

void setup() {
  //Hw initialization
  ioMg_o.Init();
  dUnitLeft_o.Init();
  dUnitRight_o.Init();
  enc1_o.Init();
  enc2_o.Init();
  i2c_o.Init();  
  serPort.Init(9600, 1000);        

  //Task initialization
  taskCtrl_o.Register(&mCtrl_o, 0);
  taskCtrl_o.Register(&nav_o, 1);
  taskCtrl_o.Register(&pos_o, 2);
  taskCtrl_o.Register(&vMap_o, 3);
  taskCtrl_o.Register(&uI_o, 4);
  taskCtrl_o.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  taskCtrl_o.Run();
}
