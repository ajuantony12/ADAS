/**
* @file ADAS_MCU.ino
* @author Hannes Bähr, Juliane Müller
* @date January 29, 2019
* @brief Main file for the NavU of the ADAS project
*
* 
* \mainpage Description
* This is the documentation of the Motor Contol Unit (MCU) of the ADAS project.
*
*/

/** Basics */
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"
#include "Timer.h"

/** Periperals */
#include "HAL_DriveUnit.h"
#include "HAL_Encoder.h"
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "HAL_Serial_IF.h"

/** Comms layer */
#include "Comm_Inertial.h"
#include "Comm_ICC.h"

/** App Layer */
#include "App_MotorCtrl.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"

Timer t;

//Hardware
  CIMUUnit    imu_o;
  CEncoder    enc1_o(CEncoder::E1);
  CEncoder    enc2_o(CEncoder::E2);
  CPWMUnit    pwmUnitLeft_o(CPWMUnit::PWM1);
  CPWMUnit    pwmUnitRight_o(CPWMUnit::PWM2);
  CSerial     iccPort(CSerial::S2, ICC_RCV_BUFF_SIZE);
//comms layer
  CICCComms   iccComms_o(iccPort);
  CMotorCtrl  mCtrl_o(imu_o, pwmUnitLeft_o, pwmUnitRight_o, enc1_o, enc2_o, iccComms_o);

void timerCallback(void* context);
void Enc_ISR_R(void);
void Enc_ISR_L(void);

void setup() {
  //Hw initialization
  Serial.begin(9600);
  DPRINTLN("Hello\n\r");
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), Enc_ISR_R, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), Enc_ISR_L, RISING);
  //readout encoder count every 150ms
  t.every(150, timerCallback, 0);
  mCtrl_o.Init();
  iccPort.Init();
  iccComms_o.Init(&mCtrl_o);
  interrupts();
}

void loop() {
  iccComms_o.Run();
  mCtrl_o.Run();
  t.update();
}

// UART2 interrupt
ISR(USART2_RX_vect)
{
 iccPort.SerialISRcommICC();
}


void timerCallback(void* context){
    mCtrl_o.readenc();
  }

void Enc_ISR_R(void){
    enc1_o.EncISR();
  }

void Enc_ISR_L(void){
    enc2_o.EncISR();
  }
