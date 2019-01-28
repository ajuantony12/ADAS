#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"

/*Periperals*/
#include "HAL_ADC.h"
#include "HAL_Serial_IF.h"

/*Comms layer*/
#include "Comm_PLS.h"
#include "Comm_ICC.h"

/*App Layer*/
#include "App_Navigation.h"
#include "App_Positioning.h"
#include "App_UserInterface.h"
#include "App_VirtualMapping.h"
#include "App_EnvironmentalData.h"

/*OSAL*/
#include "OSAL_TaskCtrl.h"

//Hardware
CSerial     plsPort(CSerial::S1, PLS_RCV_BUFF_SIZE);
CSerial     iccPort(CSerial::S2, ICC_RCV_BUFF_SIZE);
//comms layer
CPLSComms   plsCOmms_o(plsPort);
CICCComms iccComms_o(iccPort);

//Task
CNavigation nav_o(iccComms_o);
CEnvironmentalData env_o;
CVMapping vMap_o(nav_o, plsCOmms_o, 10);
/*CUser_IF uI_o;
  CPositioning pos_o;*/

/*OSAL*/
CTaskCtrl taskCtrl_o;


void setup() {
  //Hw initialization
  Serial.begin(115200);
  Serial.println("Hello\n\r");
  plsPort.Init();
  iccPort.Init();
  interrupts();

  //required for interrupt to work
  //do not know why
  delay(100);
  plsCOmms_o.Init();
  iccComms_o.Init(&nav_o);
  //Task initialization
  taskCtrl_o.Register(&vMap_o, 0);
  taskCtrl_o.Register(&nav_o, 1);
  taskCtrl_o.Register(&env_o, 2);
  /*taskCtrl_o.Register(&pos_o, 3);
  taskCtrl_o.Register(&uI_o, 4);*/
  taskCtrl_o.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  nav_o.printChangedDebugInfo();
  taskCtrl_o.Run();
  
  delay(1000);
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
