#include "App_MotorCtrl.h"
#include "Arduino.h"
#include "HAL_PWM.h" //PWM header file
#include "HAL_IMU.h" //PWM header file
#include "App_Stateflow.h" // Model's header file
#include "App_Stateflowtypes.h" // Type definitions
#include "ADAS_Debug.h"
#include "ADAS_Cfg.h"
#include "FastPID.h"
#include "Timer.h"

int n = 20;
int16_T dist = 0;
boolean control = false;
uint16_t setpoint = 124; //124 = 4km/h
uint16_t feedback;
uint16_t output_r = 0;
uint16_t output_l = 0;
uint16_t peaks_r = 0;
uint16_t peaks_l = 0;
uint16_t counted_peaks_r = 0;
uint16_t counted_peaks_l = 0;
uint16_t peak_sum_l = 0;
uint16_t peak_sum_r = 0;
uint16_t d_way = 2228; // desired way to drive in cm (1.795cm/peak) 2228==4000cm
Timer t;
float Kp = 0.2, Ki = 0.5, Kd = 0, Hz = 10;
int output_bits = 16;
bool output_signed = false;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);


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

  //Initialize encoder interrupts
  pinMode(PIN_ENC_R, INPUT_PULLUP);
  pinMode(PIN_ENC_L, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), CMotorCtrl::EncISR_R, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), CMotorCtrl::EncISR_L, RISING);

  //readout encoder count every 500ms
  t.every(500, CMotorCtrl::readenc, 0);
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
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
    } else {
      if (m_plsCOmms_o.isContaminated())
      {
        DPRINTLN("Warning Field (MtCtrl)");
        m_pwmUnitRight_o.writeMOT(LOW);
        m_pwmUnitLeft_o.writeMOT(LOW);
      }else{
        checkState();
        MotPI();
        printValues();
      }
    }
  }
  t.update();
}
// PIControl Motors
void CMotorCtrl::MotPI(void)
{
  if (control && peak_sum_l < d_way && peak_sum_r < d_way){
 //right PI control
    feedback = counted_peaks_r*4;
    output_r = myPID.step(setpoint, feedback);

    if (output_r >= 1023) {
      output_r = 1023;
    }
    
//Debug output
    Serial.print(output_r);
    Serial.print(";");
    Serial.print(counted_peaks_r);
    Serial.print(";");
    Serial.print(" Peaks: ");
    Serial.print(peak_sum_r);
   Serial.print("; ");
    
//left PI control

    feedback = counted_peaks_l*4;
    output_l = myPID.step(setpoint, feedback);

    if (output_l >= 1023) {
      output_l = 1023;
    }

//Debug output
   Serial.print(output_l);
   Serial.print(";");
    Serial.print(counted_peaks_l);
    Serial.print(";");
    Serial.print(" Peaks: ");
    Serial.print(peak_sum_l);
   Serial.println("; ");

//write to motor
    if (digitalRead(PIN_ENABLE) == HIGH) {
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
    } else {
      m_pwmUnitRight_o.writeMOT(output_r);
      m_pwmUnitLeft_o.writeMOT(output_l);
    }
    control=false;
  }else if(peak_sum_l >= d_way && peak_sum_r >= d_way){
    m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
    }
}
// Start Encoder Counting Interrupts

static void CMotorCtrl::EncISR_L(void)
{
  peaks_l++;
}

static void CMotorCtrl::EncISR_R(void)
{
  peaks_r++;
}

static void CMotorCtrl::readenc(void* context) {
  counted_peaks_r = peaks_r;
  counted_peaks_l = peaks_l;
  control = true;
  peak_sum_l = peak_sum_l + counted_peaks_l;
  peak_sum_r = peak_sum_r + counted_peaks_r;
  peaks_r = 0;
  peaks_l = 0;
}

// End Encoder Interrupts
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
      setpoint = 124;
      break;
    case 2:
      DPRINTLN("State 2: Forward");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      setpoint = 124;
      break;
    case 3:
      DPRINTLN("State 3: IDLE");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      break;
    case 4:
      DPRINTLN("State 4: Left Turn");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      
      break;
    case 5:
      DPRINTLN("State 5: Right Turn");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      setpoint = 124;
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
