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
int r = 0;
boolean pause = false;
int16_T dist = 0;
boolean control = false;
uint16_t setpoint_l = 5; //124 = 4km/h
uint16_t setpoint_r = 5; //124 = 4km/h
int curState = 3;
boolean rotated = false;
int ctrl_side = 0;
uint16_t feedback_l, feedback_r;
uint16_t output_r = 0;
int16_t adapt_r = 0;
uint16_t output_l = 0;
int16_t adapt_l = 0;
uint16_t peaks_r = 0;
uint16_t peaks_l = 0;
uint16_t counted_peaks_r = 0;
uint16_t counted_peaks_l = 0;
uint16_t peak_sum_l = 0;
uint16_t peak_sum_r = 0;
int16_t d_way = 0; // desired way to drive in cm (1.795cm/peak) 2228==4000cm
boolean startBtn = false;
boolean forward = true;
boolean backward = false;
int control_area = 50;
uint16_t lower_b, upper_b, limit_var = 0;
boolean changeDirection = false;
int16_t remindValue = 0;

int k = 0;
Timer t;
float Kp = 2.0, Ki = 2.0, Kd = 0, Hz = 10;
int output_bits = 16;
bool output_signed = false;

FastPID myPID(Kp, Ki, Kd, Hz, output_bits, output_signed);


CMotorCtrl::CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, 
CEncoder& enc1_o, CEncoder& enc2_o, CICCComms& iccComms_o):
  rtObj(),
  m_imu_o(imu_o),
  m_pwmUnitLeft_o(pwmUnitLeft_o),
  m_pwmUnitRight_o(pwmUnitRight_o),
  m_enc1_o(enc1_o),
  m_enc2_o(enc2_o),
  m_iccComms_o(iccComms_o)
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

  // Initialize stateflow
  rtObj.initialize();

  //Initialize encoder interrupts
  pinMode(PIN_ENC_R, INPUT_PULLUP);
  pinMode(PIN_ENC_L, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_R), CMotorCtrl::EncISR_R, RISING);
  attachInterrupt(digitalPinToInterrupt(PIN_ENC_L), CMotorCtrl::EncISR_L, RISING);

  //readout encoder count every 500ms
  t.every(150, CMotorCtrl::readenc, 0);
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
      if (pause)
      {
        DPRINTLN("Warning Field (MtCtrl)");
        m_pwmUnitRight_o.writeMOT(LOW);
        m_pwmUnitLeft_o.writeMOT(LOW);
      }else{
        if(startBtn){
          //getUserInput();
          checkState();
          MotPI();
          DPRINT("Turn: ");
          DPRINT(rtObj.rtU.turn);
          DPRINT("; ");
          DPRINT("peak_sum_l: ");
          DPRINT(peak_sum_l);
          DPRINT("; ");
          DPRINT("peak_sum_r: ");
          DPRINT(peak_sum_r);
          DPRINTLN("; ");
         }
      }
    }
  }
  t.update();
}
void CMotorCtrl::StraightDrive(void)
{
  if(rtObj.rtDW.is_c3_Chart == 2 || rtObj.rtDW.is_c3_Chart == 1){
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
        if(adapt_l - adapt_r < 15 && adapt_r - adapt_l > -15){
          
            if(ctrl_side == 1){
                adapt_r = 0;
                adapt_l = 0;
              }
            ctrl_side =2;
            if(rtObj.rtDW.is_c3_Chart == 2){
                  adapt_r=adapt_r-2;
                  adapt_l=adapt_l+2;
                  }else if(rtObj.rtDW.is_c3_Chart == 3){
                      adapt_r=adapt_r+2;
                      adapt_l=adapt_l-2;
                    }
          }
       
        }else if( rtObj.rtDW.curr_angle < rtObj.rtU.gyro_signal && rtObj.rtU.gyro_signal <= upper_b){
            if(adapt_l - adapt_r > -15 && adapt_r - adapt_l < 15){
              
                if(ctrl_side == 2){
                  adapt_r = 0;
                  adapt_l = 0;
                }
                ctrl_side = 1;
                if(rtObj.rtDW.is_c3_Chart == 2){
                  adapt_r=adapt_r+2;
                  adapt_l=adapt_l-2;
                  }else if(rtObj.rtDW.is_c3_Chart == 3){
                      adapt_r=adapt_r-2;
                      adapt_l=adapt_l+2;
                    }
                
            }
          }
         
    }
}
// PIControl Motors
void CMotorCtrl::MotPI(void)
{
    if (control && rtObj.rtDW.is_c3_Chart != 3){      
      StraightDrive();
 //right PI control
    feedback_r = counted_peaks_r;
    output_r = myPID.step(setpoint_r, feedback_r);

    if (output_r >= 1023) {
      output_r = 1023;
    }
   
    
//left PI control

    feedback_l = counted_peaks_l;
    output_l = myPID.step(setpoint_l, feedback_l);

    if (output_l >= 1023) {
      output_l = 1023;
    }
          
//write to motor
    if (digitalRead(PIN_ENABLE) == HIGH || rtObj.rtDW.is_c3_Chart == 3) {
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      output_l = output_r = 0;
    } else {
      m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
      m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
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

void CMotorCtrl::startRotation(sint16_t angle){
    rtObj.rtU.turn = angle;
    rtObj.rtU.dist = 0;
  }

void CMotorCtrl::setDistance(sint16_t dist){
  rtObj.rtU.turn = 0;
  peak_sum_r = peak_sum_l = 0;
  if(rtObj.rtU.dist >= 0 && dist >= 0){
    rtObj.rtU.dist = d_way = dist;
    }else if(rtObj.rtU.dist <= 0 && dist < 0){
        rtObj.rtU.dist= dist;
        d_way = abs(dist);
      }else if(rtObj.rtU.dist > 0 && dist < 0){
          rtObj.rtU.dist= 0;
          d_way = 0;
          changeDirection = true;
          remindValue = dist;
        }else if(rtObj.rtU.dist < 0 && dist > 0){
          rtObj.rtU.dist= 0;
          d_way = 0;
          changeDirection = true;
          remindValue = dist;
        }
  }

void CMotorCtrl::pauseDrive(void){
    m_pwmUnitRight_o.writeMOT(LOW);
    m_pwmUnitLeft_o.writeMOT(LOW);
    pause = true;
  }
void CMotorCtrl::contDrive(void){
    pause = false;
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
      if(curState != 1){
          setpoint_l = 3;
          setpoint_r = 3;
        }
      curState = 1;
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 2:
      DPRINTLN("State 2: Forward");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      if(curState != 2){
          setpoint_l = 5;
          setpoint_r = 5;
        }
      curState = 2;
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 3:
      DPRINTLN("State 3: IDLE");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      if(curState != 3){
          setpoint_l = 0;
          setpoint_r = 0;
        }
      if(curState == 4 || curState == 5){
          //Feedback function for completed rotation
          DPRINT("Rotation reached!");
          rtObj.rtU.turn = 0;
          rotated = true;
          m_iccComms_o.addTxMsg(ICC_CMD_FB_ROT, 0);
        }else if(curState == 1 || curState == 2){
            //Feedback function for completed distance
            rtObj.rtU.dist = 0;
            DPRINT("Distance reached!");
           
            m_iccComms_o.addTxMsg(ICC_CMD_FB_DIST, 0);
          }

      if(changeDirection){
          k++;
          if(k == 50){
              changeDirection = false;
              setDistance(remindValue);
              remindValue = 0;
              k = 0;
            }
        }
      curState = 3;
      peak_sum_l = peak_sum_r = 0;
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      break;
    case 4:
      DPRINTLN("State 4: Left Turn");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      if(curState != 4){
          setpoint_l = 3;
          setpoint_r = 3;
        }
      curState = 4;
      break;
    case 5:
      DPRINTLN("State 5: Right Turn");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      if(curState != 5){
          setpoint_l = 3;
          setpoint_r = 3;
        }
      curState = 5;
      break;
  }
}


#ifdef ADAS_DEBUG
void CMotorCtrl::getUserInput(void) {}

void CMotorCtrl::printValues(void) {
  DPRINT("peak_sum_l: ");
  DPRINT(peak_sum_l);
  DPRINT("; ");
  DPRINT("peak_sum_r: ");
  DPRINT(peak_sum_r);
  DPRINT("; ");
  DPRINT("desired distance: ");
  DPRINTLN(d_way);
  DPRINT("gyro: ");
  DPRINT(rtObj.rtU.gyro_signal);
  DPRINT("; ");
  DPRINT("curr_angle: ");
  DPRINT(rtObj.rtDW.curr_angle);
  DPRINTLN("; ");
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
