#include "App_MotorCtrl.h"
#include "Arduino.h"
#include "HAL_PWM.h" //PWM header file
#include "HAL_IMU.h" //PWM header file
#include "App_Stateflow.h" // Model's header file
#include "App_Stateflowtypes.h" // Type definitions
#include "ADAS_Debug.h"
#include "ADAS_Cfg.h"

int n = 20;
int16_T dist = 0;

CMotorCtrl::CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, CPLSComms& plsCOmms_o, CEncoder& enc1_o, CEncoder& enc2_o):
  rtObj(),
  m_imu_o(imu_o),
  m_pwmUnitLeft_o(pwmUnitLeft_o),
  m_pwmUnitRight_o(pwmUnitRight_o),
  m_plsCOmms_o(plsCOmms_o),
  m_enc1_o(enc1_o),
  m_enc2_o(enc2_o)
{

}
CMotorCtrl::~CMotorCtrl() {
  //do nothing
}

void CMotorCtrl::Init(void)
{
  //Setup PWM
  pinMode(PIN_ENABLE, OUTPUT);
  pinMode(PIN_DIRECTION_R, OUTPUT);
  pinMode(PIN_DIRECTION_L, OUTPUT);

  DPRINTLN("Motor Control Init");

  digitalWrite(PIN_ENABLE, LOW);
  digitalWrite(PIN_DIRECTION_R, HIGH);
  digitalWrite(PIN_DIRECTION_L, HIGH);


  m_pwmUnitLeft_o.setupPWM16();
  m_pwmUnitRight_o.setupPWM16();
  m_imu_o.Init();

  m_pwmUnitLeft_o.writeMOT(1023);
  m_pwmUnitRight_o.writeMOT(1023);

  // Initialize stateflow
  rtObj.initialize();
}

void CMotorCtrl::Run(void)
{
  //Stateflow
  if (rtmGetErrorStatus(rtObj.getRTM()) != (NULL)) {
    //  Called when error in stateflow
    DPRINTLN("StateFlow Error!");
    while (true) {}

  } else {

    //Set Gyro Signal
    rtObj.rtU.gyro_signal = m_imu_o.ReturnGyro();

    getUserInput();

    n++;

    // Call stateflow
    rt_OneStep();

    //PWM
    if (digitalRead(PIN_ENABLE) == HIGH) {
      m_pwmUnitRight_o.writeMOT(1023);
      m_pwmUnitLeft_o.writeMOT(1023);
    } else {
      if (m_plsCOmms_o.isContaminated())
      {
        checkState();
      }
    }

    printValues();
  }
}

void CMotorCtrl::Stop(void)
{
  //do nothing
}

void CMotorCtrl::checkState(void) {
  switch (rtObj.rtDW.is_c3_Chart) {
    case 1:
      DPRINTLN("State 1: Backward");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      m_pwmUnitRight_o.writeMOT(950);
      m_pwmUnitLeft_o.writeMOT(950);
      break;
    case 2:
      DPRINTLN("State 2: Forward");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      m_pwmUnitRight_o.writeMOT(950);
      m_pwmUnitLeft_o.writeMOT(950);
      break;
    case 3:
      DPRINTLN("State 3: IDLE");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      m_pwmUnitRight_o.writeMOT(1023);
      m_pwmUnitLeft_o.writeMOT(1023);
      break;
    case 4:
      DPRINTLN("State 4: Left Turn");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      m_pwmUnitRight_o.writeMOT(950);
      m_pwmUnitLeft_o.writeMOT(950);
      break;
    case 5:
      DPRINTLN("State 5: Right Turn");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      m_pwmUnitRight_o.writeMOT(950);
      m_pwmUnitLeft_o.writeMOT(950);
      break;
  }
}


#ifdef ADAS_DEBUG
void CMotorCtrl::getUserInput(void) {
  if (n == 20) {
    rtObj.rtU.turn = -15;
    rtObj.rtU.dist = 0;
  }
  if (n == 40) {
    rtObj.rtU.turn = 0;
    rtObj.rtU.dist = 0;
  }
  if (n == 60) {
    rtObj.rtU.turn = -15;
    rtObj.rtU.dist = 0;
  }
  if (n == 80) {
    rtObj.rtU.turn = 0;
    rtObj.rtU.dist = 0;
  }
}

void CMotorCtrl::printValues(void) {
  DPRINTLN("____________________________________________________________________________ ");
  DPRINTLN("Inputs and local Variables: ");
  DPRINTLN("____________________________________________________________________________ ");
  DPRINT("gyro_signal: ");
  DPRINT(rtObj.rtU.gyro_signal);
  DPRINT(" || Curr_Angle: ");
  DPRINT(rtObj.rtDW.curr_angle);
  DPRINT(" || Turn: ");
  DPRINT(rtObj.rtU.turn);
  DPRINT(" || op: ");
  DPRINT(rtObj.rtDW.op);
  DPRINT(" || distance: ");
  DPRINTLN(rtObj.rtU.dist);
  DPRINTLN("____________________________________________________________________________ ");
  DPRINTLN("Outputs: ");
  DPRINTLN("____________________________________________________________________________ ");
  DPRINT("Mot_l: ");
  DPRINT(rtObj.rtY.mot_l);
  DPRINT(" || Dir_l: ");
  DPRINT(rtObj.rtY.dir_l);
  DPRINT(" || Mot_r: ");
  DPRINT(rtObj.rtY.mot_r);
  DPRINT(" || Dir_r: ");
  DPRINTLN(rtObj.rtY.dir_r);
}
#endif

// Function to perform a stateflow calculation
void CMotorCtrl::rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model
  rtObj.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}
