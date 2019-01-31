/**
* @file App_MotorCtrl.cpp
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for environmental data
*
*/

#include "App_MotorCtrl.h"
#include "Arduino.h"
#include "HAL_PWM.h" 
#include "HAL_IMU.h" 
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"
#include "ADAS_Debug.h"
#include "ADAS_Cfg.h"
#include "FastPID.h"
#include "Timer.h"

uint16_t peaks_r, peaks_l, counted_peaks_r, counted_peaks_l, peak_sum_l, peak_sum_r = 0;
boolean control = false;
Timer t;
FastPID myPID(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);


//! Constructor of CMotorCtrl
CMotorCtrl::CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, CICCComms& iccComms_o):
  rtObj(),
  m_imu_o(imu_o),
  m_pwmUnitLeft_o(pwmUnitLeft_o),
  m_pwmUnitRight_o(pwmUnitRight_o),
  m_iccComms_o(iccComms_o),
  distance(0), pause(false), speed_set(false), spd(2), setpoint_l(5), setpoint_r(5), curState(3),
  ctrl_side(0), feedback_l(0), feedback_r(0), output_r(0), adapt_r(0), output_l(0), adapt_l(0),
  d_way(0), startBtn(false), control_area(50), lower_b(0), upper_b(0), limit_var(0),
  changeDirection(false), remindValue(0), k(0)
{

}
//! Desctructor of CMotorCtrl
CMotorCtrl::~CMotorCtrl() {
  //do nothing
}

//! Initialization function of CMotorCtrl
/*!
  Function set up the PWM signal, the IMU, initializes the stateflow, and initiaizes the encoder interrupts.
  Furthermore a timer is activated which reads out the counted peaks of the encoders every 150ms. 
*/
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

  //Setup IMU
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

//! Run function of CMotorCtrl which is executed in every loop
/*!
  The function checks if the motor control stateflow is valid. If yes it is reading out the 
  gyro sensor and performs a stateflow calculation. If the H-bridge is enabled and the start 
  button was pressed it checks for the current state and runs the PI control. At the end of 
  the function the timer for the readout function of the encoder is updated. 
*/
void CMotorCtrl::Run(void)
{
  //Check if Stateflow is valid
  if (rtmGetErrorStatus(rtObj.getRTM()) != (NULL)) {
    //  Called when error in stateflow
    DPRINTLN("StateFlow Error!");
    while (true) {}
  } else {
    //Set Gyro Signal
    rtObj.rtU.gyro_signal = m_imu_o.ReturnGyro();
    
    //Safety function to start the motor control
    if(digitalRead(Btn_Start) == HIGH){
        startBtn = true;
      }
 
    // Call stateflow
    rt_OneStep();

    //Check if H-bridge is enabled
    if (digitalRead(PIN_ENABLE) == HIGH) {
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
    } else {
      if (pause)
      {
        m_pwmUnitRight_o.writeMOT(LOW);
        m_pwmUnitLeft_o.writeMOT(LOW);
      }else{
        if(startBtn){
          //Check for current state and run the PI control
          checkState();
          MotPI();
         }
      }
    }
  }
  //Update the timer
  t.update();
}
//! Position control of CMotorCtrl which is executed every 150ms if the forward or backward driving is active
/*!
  The function checks if the current gyro sensor value differs from the angle which was present while entering 
  the forward or backward state. The function increases or decreases two adaption variables which are later
  added to the ouput value written on the motors. 
*/
void CMotorCtrl::StraightDrive(void)
{
  //Checks if forward or backward state is active
  if(rtObj.rtDW.is_c3_Chart == 2 || rtObj.rtDW.is_c3_Chart == 1){
    //Set upper border
     if(rtObj.rtDW.curr_angle + control_area >= 360){
          upper_b = rtObj.rtDW.curr_angle + control_area - 360;
        }else{
          upper_b = rtObj.rtDW.curr_angle + control_area;
      }
      //Set lower border 
      if(rtObj.rtDW.curr_angle - control_area < 0){
          lower_b = rtObj.rtDW.curr_angle - control_area + 360;
        }else{
            lower_b = rtObj.rtDW.curr_angle - control_area;
      }
      //Checks if the gyro signal is smaller than the initial angle and higher than the lower border (left drift)
      if(rtObj.rtU.gyro_signal < rtObj.rtDW.curr_angle || rtObj.rtU.gyro_signal >= lower_b){
        //Set a maximum difference between the adaption variables to prevent too high speed
        if(adapt_l - adapt_r > -15 && adapt_r - adapt_l < 15 || ctrl_side == 1){
            // Reset adaption variables at first enter of left drift adaption
            if(ctrl_side == 1){
                adapt_r = 0;
                adapt_l = 0;
              }
            ctrl_side =2;
            // Adapt the variables for forward state
            if(rtObj.rtDW.is_c3_Chart == 2){
                  adapt_r=adapt_r+2;
                  adapt_l=adapt_l-2;
                  // Adapt the variables for backward state (reversed)
                  }else if(rtObj.rtDW.is_c3_Chart == 1){
                      adapt_r=adapt_r-2;
                      adapt_l=adapt_l+2;
                    }
          }
       //Checks if the gyro signal is bigge than the initial angle and smaller than the upper border (right drift)
        }else if( rtObj.rtDW.curr_angle < rtObj.rtU.gyro_signal && rtObj.rtU.gyro_signal <= upper_b){
          //Set a maximum difference between the adaption variables to prevent too high speed
            if(adapt_l - adapt_r < 15 && adapt_r - adapt_l > -15 || ctrl_side == 2){
              // Reset adaption variables at first enter of right drift adaption
                if(ctrl_side == 2){
                  adapt_r = 0;
                  adapt_l = 0;
                }
                ctrl_side = 1;
                // Adapt the variables for forward state
                if(rtObj.rtDW.is_c3_Chart == 2){
                  adapt_r=adapt_r-2;
                  adapt_l=adapt_l+2;
                  // Adapt the variables for backward state (reversed)
                  }else if(rtObj.rtDW.is_c3_Chart == 1){
                      adapt_r=adapt_r+2;
                      adapt_l=adapt_l-2;
                    }
                
            }
          }
    }
}
//! PI control of CMotorCtrl which is executed every 150ms by the timer
/*!
  Function executes a PI controller for the left and right DC motor. As a feedback the counted peaks 
  per 150ms are used. It is executing the position control. 
*/
void CMotorCtrl::MotPI(void)
{
  //Checks if the  
    if (control && rtObj.rtDW.is_c3_Chart != 3){      
    StraightDrive();
    //Executing right PI control
    feedback_r = counted_peaks_r;
    output_r = myPID.step(setpoint_r, feedback_r);

    if (output_r >= 1023) {
      output_r = 1023;
    }
   
    //Executing left PI control
    feedback_l = counted_peaks_l;
    output_l = myPID.step(setpoint_l, feedback_l);

    if (output_l >= 1023) {
      output_l = 1023;
    }
          
    checkOverflow();
    
    control=false;
  }
}

void CMotorCtrl::checkOverflow(void){
    if (digitalRead(PIN_ENABLE) == HIGH || rtObj.rtDW.is_c3_Chart == 3) {
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      output_l = output_r = 0;
    } else {
      if(adapt_r < 0){
          if(abs(adapt_r) > output_r*2){
              m_pwmUnitRight_o.writeMOT(output_r*2);
              m_pwmUnitLeft_o.writeMOT(output_l*2);
            }else{
                m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
                m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
              }
        }else if(adapt_l < 0){
            if(abs(adapt_l) > output_l*2){
                m_pwmUnitRight_o.writeMOT(output_r*2);
                m_pwmUnitLeft_o.writeMOT(output_l*2);
              }else{
                  m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
                  m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
                }
          }else{
              m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
              m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
            }
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
    rtObj.rtU.dist = d_way = 0 ;
  }

void CMotorCtrl::setDistance(sint16_t dist){
  //1 peak = 1.795cm;
  distance = dist * (1/1.795);
  rtObj.rtU.turn = 0;
  peak_sum_r = peak_sum_l = 0;
  if(rtObj.rtU.dist >= 0 && dist >= 0){
    rtObj.rtU.dist = d_way = distance;
    }else if(rtObj.rtU.dist <= 0 && dist < 0){
        rtObj.rtU.dist = distance;
        DPRINT("Distance Stateflow: ");
        DPRINTLN(rtObj.rtU.dist);
        d_way = -distance;
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

void CMotorCtrl::setPISetpoint(uint16_t setpnt){
  if(setpnt >= 1000){
    setpoint_l = setpoint_r = 8;
    spd = 8;
    }else{
    setpoint_l = setpoint_r = spd = (setpnt/120); // mm/s in peaks per 150ms 
    speed_set = true;
    }
    
  }
void CMotorCtrl::contDrive(void){
    pause = false;
  }

void CMotorCtrl::checkState(void) {
  switch (rtObj.rtDW.is_c3_Chart) {
    case 1:
      DPRINTLN("State 1: Backward");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      if(curState != 1 && !speed_set){
          myPID.clear();
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          setpoint_l = spd;
          setpoint_r = spd;
          DPRINT("SETPOINT SET TO : ");
          DPRINTLN(spd);
        }
      curState = 1;
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 2:
      //DPRINTLN("State 2: Forward");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      if(curState != 2 && !speed_set){
          myPID.clear();
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 2;
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 3:
      //DPRINTLN("State 3: IDLE");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      if(curState != 3){
          myPID.clear();
          setpoint_l = 0;
          setpoint_r = 0;
        }
      if(speed_set){
          speed_set = false;
        }
      if(curState == 4 || curState == 5){
          //Feedback function for completed rotation
          DPRINT("Rotation reached!");
          rtObj.rtU.turn = 0;
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
      //DPRINTLN("State 4: Left Turn");
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      if(curState != 4 && !speed_set){
          myPID.clear();
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 4;
      break;
    case 5:
      //DPRINTLN("State 5: Right Turn");
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      if(curState != 5 && !speed_set){
          myPID.clear();
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 5;
      break;
  }
}

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
