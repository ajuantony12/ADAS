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
uint16_t setpoint_l = 15; //124 = 4km/h
uint16_t setpoint_r = 15; //124 = 4km/h
int curState = 3;
uint16_t feedback_l, feedback_r;
uint16_t output_r = 0;
uint16_t output_l = 0;
uint16_t peaks_r = 0;
uint16_t peaks_l = 0;
uint16_t counted_peaks_r = 0;
uint16_t counted_peaks_l = 0;
uint16_t peak_sum_l = 0;
uint16_t peak_sum_r = 0;
uint16_t d_way = 812; // desired way to drive in cm (1.795cm/peak) 2228==4000cm
boolean startBtn = false;
boolean forward = true;
boolean backward = false;
int control_area = 30;
uint16_t lower_b, upper_b, limit_var = 0;

int k = 0;
Timer t;
float Kp = 0.6, Ki = 0.6, Kd = 0.15, Hz = 10;
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

  m_pwmUnitLeft_o.writeMOT(LOW);
  m_pwmUnitRight_o.writeMOT(LOW);

  // Initialize stateflow
  rtObj.initialize();

  //Initialize encoder interrupts
  pinMode(PIN_ENC_R, INPUT_PULLUP);
  pinMode(PIN_ENC_L, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), CMotorCtrl::EncISR_R, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), CMotorCtrl::EncISR_L, RISING);

  //readout encoder count every 500ms
  t.every(250, CMotorCtrl::readenc, 0);
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
    
    if(digitalRead(Btn_Start) == HIGH){
      
        startBtn = true;
      }
    

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
        if(startBtn){
          getUserInput();
          checkState();
          MotPI();
          //printValues();
         }
      }
    }
  }
  t.update();
}
void CMotorCtrl::StraightDrive(void)
{
  if(rtObj.rtDW.is_c3_Chart == 2){
            if(rtObj.rtDW.curr_angle + control_area >= 360){
          upper_b = rtObj.rtDW.curr_angle + control_area - 360;
        }else{
          upper_b = rtObj.rtDW.curr_angle + control_area;
      }
      if(rtObj.rtDW.curr_angle - control_area < 0){
          lower_b = rtObj.rtDW.curr_angle - control_area + 360;
        }else{
            lower_b = rtObj.rtDW.curr_angle - control_area;
      }

      if(rtObj.rtU.gyro_signal < rtObj.rtDW.curr_angle || rtObj.rtU.gyro_signal >= lower_b){
        setpoint_l--;
        setpoint_r++;
        }else if( rtObj.rtDW.curr_angle < rtObj.rtU.gyro_signal <= upper_b){
            setpoint_l++;
            setpoint_r--;
          }
       DPRINT("Curr angle : ");
       DPRINT(rtObj.rtDW.curr_angle);
       DPRINT(";");
       DPRINT("Gyro : ");
       DPRINT(rtObj.rtU.gyro_signal);
       DPRINTLN("; ");
       DPRINT("Upper border : ");
       DPRINT(upper_b);
       DPRINT(";");
       DPRINT("Lower border : ");
       DPRINT(lower_b);
       DPRINTLN("; ");
  }
}
// PIControl Motors
void CMotorCtrl::MotPI(void)
{
    if (control && peak_sum_r < d_way && peak_sum_l < d_way ){      
 //right PI control
    feedback_r = counted_peaks_r;
    output_r = myPID.step(setpoint_r, feedback_r);

    if (output_r >= 1023) {
      output_r = LOW;
    }
   
    
//left PI control

    feedback_l = counted_peaks_l;
    output_l = myPID.step(setpoint_l, feedback_l);

    if (output_l >= 1023) {
      output_l = 1023;
    }

//Debug output
   DPRINT("Setpoint left: ");
   DPRINT(setpoint_l);
   DPRINT(";");
   DPRINT("Setpoint right: ");
   DPRINT(setpoint_r);
   DPRINTLN("; ");
    
   DPRINT("Feedback left: ");
   DPRINT(feedback_l);
   DPRINT(";");
   DPRINT("Feedback right: ");
   DPRINT(feedback_r);
   DPRINTLN("; ");
   
   DPRINT("Output left: ");
   DPRINT(output_l);
   DPRINT(";");
   DPRINT("Output right: ");
   DPRINT(output_r);
   DPRINTLN("; ");

//write to motor
    if (digitalRead(PIN_ENABLE) == HIGH || rtObj.rtDW.is_c3_Chart == 3) {
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      output_l = output_r = 0;
    } else {
      m_pwmUnitRight_o.writeMOT(output_r*2);
      m_pwmUnitLeft_o.writeMOT(output_l*2);
    }
    control=false;
  }else if(peak_sum_l >= d_way && peak_sum_r >= d_way){
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      output_l = output_r = 0;
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
      if(curState != 1){
          setpoint_l = 15;
          setpoint_r = 15;
        }
      curState = 1;
      break;
    case 2:
      DPRINTLN("State 2: Forward");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      if(curState != 2){
          setpoint_l = 15;
          setpoint_r = 15;
        }
      curState = 2;
      break;
    case 3:
      DPRINTLN("State 3: IDLE");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      curState = 3;
      setpoint_l = 0;
      setpoint_r = 0;
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      break;
    case 4:
      DPRINTLN("State 4: Left Turn");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      setpoint_l = 50;
      setpoint_r = 50;
      break;
    case 5:
      DPRINTLN("State 5: Right Turn");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      setpoint_l = 50;
      setpoint_r = 50;
      break;
  }
}


#ifdef ADAS_DEBUG
void CMotorCtrl::getUserInput(void) {
   if(forward && peak_sum_r < d_way && peak_sum_l < d_way){
   rtObj.rtU.turn = 0;
   rtObj.rtU.dist = d_way;
   }else if(forward && peak_sum_r >= d_way && peak_sum_l >= d_way){
      rtObj.rtU.turn = 0;
      rtObj.rtU.dist = 0;
//      k++;
//      if(k == 20){
//         forward = false;
//         backward = true;
//         peak_sum_r =  peak_sum_l = 0;
//         k=0;
//       }
     }

// if(backward && peak_sum_r <= d_way && peak_sum_l <= d_way){
//     rtObj.rtU.turn = 0;
//     rtObj.rtU.dist = -d_way;
//   }else if(backward && peak_sum_r >= d_way && peak_sum_l >= d_way){
//       rtObj.rtU.turn = 0;
//       rtObj.rtU.dist = 0;
//       startBtn = false;
//       peak_sum_r = peak_sum_l = 0;
//     }
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
