/**
* @file App_MotorCtrl.cpp
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for environmental data
*
*/
// ##### Includes #####
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
  per 150ms are used. It is executing the position control and the function to check if the calculated output 
  plus the adaption variables from the position control would lead to an overflow. 
*/
void CMotorCtrl::MotPI(void)
{
  //Checks if the control variable is true and the IDLE state is not active
    if (control && rtObj.rtDW.is_c3_Chart != 3){      
      //Executes the position control
      StraightDrive();
      
      //Executing right PI control
      feedback_r = counted_peaks_r;
      output_r = myPID.step(setpoint_r, feedback_r);
      //Prevent writing a number to the motor which is out of range of the 10bit PWM signal
      if (output_r >= 1023) {
        output_r = 1023;
      }
     
      //Executing left PI control
      feedback_l = counted_peaks_l;
      output_l = myPID.step(setpoint_l, feedback_l);
      //Prevent writing a number to the motor which is out of range of the 10bit PWM signal
      if (output_l >= 1023) {
        output_l = 1023;
      }

      //Executes the function which checks for an overflow
      checkOverflow();

      //set control variable to false 
      control=false;
  }
}

//! Function to prevent writing a negative number to the DC motors
/*!
  In this function it is checked if the absolute value of the negative adaption variable is higher than the output variable,
  written to the motor. If this is true the adaption variables are ignored. It is also checking if the H-bridge is enabled 
  or if the IDLE state is active. 
*/
void CMotorCtrl::checkOverflow(void){
    //Checks if the H-bridge is enables or the IDLE state is active
    if (digitalRead(PIN_ENABLE) == HIGH || rtObj.rtDW.is_c3_Chart == 3) {
      //Stop the motor if the condition is true
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      output_l = output_r = 0;
    } else {
      //Checks if the right adaption is negative
      if(adapt_r < 0){
        //Checks if the absolute of the negative value is higher than the output which is written to the motor
          if(abs(adapt_r) > output_r*2){
            //Ignore adaption variables
              m_pwmUnitRight_o.writeMOT(output_r*2);
              m_pwmUnitLeft_o.writeMOT(output_l*2);
            }else{
                // Write to motor with adaption variables
                m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
                m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
              }
        //Checks if the left adaption is negative
        }else if(adapt_l < 0){
          //Checks if the absolute of the negative value is higher than the output which is written to the motor
            if(abs(adapt_l) > output_l*2){
              //Ignore adaption variables
                m_pwmUnitRight_o.writeMOT(output_r*2);
                m_pwmUnitLeft_o.writeMOT(output_l*2);
              }else{
                // Write to motor with adaption variables
                  m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
                  m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
                }
          }else{
            // Write to motor with adaption variables
              m_pwmUnitRight_o.writeMOT(output_r*2+adapt_r);
              m_pwmUnitLeft_o.writeMOT(output_l*2+adapt_l);
            }
    }
  }

//! Interrupt Service Routine of the left encoder
/*!
  In this ISR the peaks of the left encoder are counted.
*/
static void CMotorCtrl::EncISR_L(void)
{
  peaks_l++;
}

//! Interrupt Service Routine of the right encoder
/*!
  In this ISR the peaks of the right encoder are counted.
*/
static void CMotorCtrl::EncISR_R(void)
{
  peaks_r++;
}

//! Function of the CMotorCtrl where the sum of all peaks and the peaks per 150ms are read out
/*!
  Function is called every 150ms and resets the count of the peaks which are used in the ISR's.
  It also reads out the count of the peaks per 150ms and sums them up. The control variable is set to 
  true which enables the PI controller. 
*/
static void CMotorCtrl::readenc(void* context) {
  // Peaks per 150ms are saved
  counted_peaks_r = peaks_r;
  counted_peaks_l = peaks_l;
  // PI controller gets enables
  control = true;
  // Peaks are getting summed up
  peak_sum_l = peak_sum_l + counted_peaks_l;
  peak_sum_r = peak_sum_r + counted_peaks_r;
  // Reset of the counting variable of the ISR's
  peaks_r = 0;
  peaks_l = 0;
}

//! API Function of the CMotorCtrl to start a rotation
/*!
  The function sets the turn variable of the motor control stateflow to the transmitted angle and the distance to zero.
  A negative angle would cause a rotation to the left and a positive angle would cause a rotation to the right.
*/
void CMotorCtrl::startRotation(sint16_t angle){
    rtObj.rtU.turn = angle;
    rtObj.rtU.dist = d_way = 0 ;
  }


//! API Function of the CMotorCtrl to start driving
/*!
  The function calculates from the transmitted distance in cm, the peaks which have to be summed up to reach the distance.
  The summation of all peaks are resetted and the turn variable is set to zero. Then the function checks if the distance value
  of the stateflow was setted previously and if the transmitted distance value is negative or positive. If the setted value is 
  positive and the transmitted one is negative a state change from forward drive to backward drive must be performed. Same if
  the setted value is negative and the transmitted one is positive.  
*/
void CMotorCtrl::setDistance(sint16_t dist){
  //Calculation of the needed peaks for the transmitted distance in cm
  distance = dist * (1/1.795);
  // Set turn parameter of the stateflow to zero
  rtObj.rtU.turn = 0;
  // Reset of the summed up peaks
  peak_sum_r = peak_sum_l = 0;
  // Checks if the setted distance and the transmitted distance are greater or equal to zero
  if(rtObj.rtU.dist >= 0 && dist >= 0){
    // Sets distance parameter of the stateflow and CMotorCtrl class to calculated peaks
    rtObj.rtU.dist = d_way = distance;
    // Checks if the setted distance is smaller or equal to zero and the transmitted distance is smaller than zero
    }else if(rtObj.rtU.dist <= 0 && dist < 0){
        // Set distance parameter of the stateflow to calculated peaks
        rtObj.rtU.dist = distance;
        // Set CMotorCtrl distance parameter to positive value of calculated peaks
        d_way = -distance;
      //Checks if setted distance and transmitted distance are greater than zero
      }else if(rtObj.rtU.dist > 0 && dist < 0){
          //  Set distance parameter of stateflow to zero (state will change to IDLE)
          rtObj.rtU.dist= 0;
          // Set CMotorCtrl distance parameter to zero
          d_way = 0;
          // Set direction change to true and saves the latest transmitted distance to a variable
          changeDirection = true;
          remindValue = dist;
          // Checks if setted distance and transmitted distance are smaller than zero
        }else if(rtObj.rtU.dist < 0 && dist > 0){
          //  Set distance parameter of stateflow to zero (state will change to IDLE)
          rtObj.rtU.dist= 0;
          // Set CMotorCtrl distance parameter to zero
          d_way = 0;
          // Set direction change to true and saves the latest transmitted distance to a variable
          changeDirection = true;
          remindValue = dist;
        }
  }

//! API Function of the CMotorCtrl to pause the current action of the motor control
/*!
    Function stops the DC motors and set a pause variable to true.
*/
void CMotorCtrl::pauseDrive(void){
    m_pwmUnitRight_o.writeMOT(LOW);
    m_pwmUnitLeft_o.writeMOT(LOW);
    pause = true;
  }

//! API Function of the CMotorCtrl to set a speed for the motors
/*!
    Function checks first if the desired speed is too high. If the transmitted setpoint is ok it is converted from mm/s into 
    peaks per 150ms. Then the setpoint of the PI controllers setpoint is changed to this value. 
*/
void CMotorCtrl::setPISetpoint(uint16_t setpnt){
  // Checks if the transmitted speed is too high
  if(setpnt >= 1000){
    // Set to a safe setpoint
    setpoint_l = setpoint_r = 5;
    spd = 5;
    }else{
      // Convert transmitted speed from mm/s into peaks per 150ms and set PI controllers setpoint
      setpoint_l = setpoint_r = spd = (setpnt/120); // mm/s in peaks per 150ms 
      speed_set = true;
    }
    
  }

//! API Function of the CMotorCtrl to continue the previously paused action of the motor control
/*!
    Function sets the pause variable to false and the motor control will continue with the present action.
*/
void CMotorCtrl::contDrive(void){
    pause = false;
  }

//! Function of the CMotorCtrl to check the active state of the stateflow
/*!
  Function checks which state of the stateflow is active. Five states are possible: Backward, Forward, IDLE, Left Turn, 
  and Right Turn. When first entering a state the PI controller will be resetted and reconfigured. Then the setpoint of the
  controller is setted. In the backward and forward state is checked if the distance is reached. If true it will adapt the 
  parameters to change into IDLE state. The IDLE state also checks if the previous state was a turn state or a drive state. 
  It will send a "rotation done" or "distance reached" message to the navigation unit accordingly. Another thing which is 
  handled by the IDLE state is the direction change (more information in setDistance()). If the direction change is true it 
  will call setDirection() after a small time with the reminded value. Main aspect of this function is to active the motors
  and set the directions according to the active state. 
*/
void CMotorCtrl::checkState(void) {
  switch (rtObj.rtDW.is_c3_Chart) {
    case 1:
      // Case Backward state
      // Set the direction pins of the H-bridge to drive backwards
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      // Checks if first enter of this state is true and the speed is not set via the API function
      if(curState != 1 && !speed_set){
          // Clear the PI controller
          myPID.clear();
          // Reconfigure the PI controller
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          // Set setpoint of the PI controller
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 1;
      // Check if desired distance is reached
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
          // Adapt stateflow and CMotorCtrl parameters to perform a state change to IDLE
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 2:
       // Case Forward state
       // Set the direction pins of the H-bridge to drive forward
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, LOW);
      // Checks if first enter of this state is true and the speed is not set via the API function
      if(curState != 2 && !speed_set){
        // Clear the PI controller
          myPID.clear();
          // Reconfigure the PI controller
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          // Set setpoint of the PI controller
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 2;
      // Check if desired distance is reached
      if(peak_sum_r >= d_way && peak_sum_l >= d_way){
        // Adapt stateflow and CMotorCtrl parameters to perform a state change to IDLE
          rtObj.rtU.dist=0;
          d_way=0;
        }
      break;
    case 3:
      // Case IDLE state
      // Set direction pins of H-bridge to high as default
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      // Checks if first enter of this state
      if(curState != 3){
          // Reset PI controller
          myPID.clear();
          // Set setpoint of PI controller to zero
          setpoint_l = 0;
          setpoint_r = 0;
        }
      // Checks if previous state was a turn state
      if(curState == 4 || curState == 5){
          DPRINT("Rotation reached!");
          // Set turn parameter of stateflow to zero
          rtObj.rtU.turn = 0;
          //Feedback for completed rotation
          m_iccComms_o.addTxMsg(ICC_CMD_FB_ROT, 0);
      //Checks if previous state was a drive state
        }else if(curState == 1 || curState == 2){
            // Sets distance parameter of stateflow to zero
            rtObj.rtU.dist = 0;
            DPRINT("Distance reached!");
            //Feedback function for completed distance
            m_iccComms_o.addTxMsg(ICC_CMD_FB_DIST, 0);
          }
      // Checks if a direction change should be performed
      if(changeDirection){
          // Starts counter
          k++;
          if(k == 50){
              // Set direction change to false
              changeDirection = false;
              // Call API function with reminded value
              setDistance(remindValue);
              // Reset reminded value and counter
              remindValue = 0;
              k = 0;
            }
        }
      curState = 3;
      // Reset summed up peaks
      peak_sum_l = peak_sum_r = 0;
      // Stop motors
      m_pwmUnitRight_o.writeMOT(LOW);
      m_pwmUnitLeft_o.writeMOT(LOW);
      break;
    case 4:
      // Case Left Turn state
      // Set direction pins of H-bridge to a left turn
      digitalWrite(PIN_DIRECTION_L, LOW);
      digitalWrite(PIN_DIRECTION_R, LOW);
      // Checks if first enter of this state is true and the speed is not set via the API function
      if(curState != 4 && !speed_set){
          // Reset PI controller
          myPID.clear();
          // Reconfigure PI controller
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          // Set setpoint of the PI controller
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 4;
      break;
    case 5:
      // Case Right Turn state
      // Set direction pins of H-bridge to a right turn
      digitalWrite(PIN_DIRECTION_L, HIGH);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      // Checks if first enter of this state is true and the speed is not set via the API function
      if(curState != 5 && !speed_set){
          // Reset PI controller
          myPID.clear();
          // Reconfigure PI controller
          myPID.configure(KP_VALUE, KI_VALUE, KD_VALUE, HZ_VALUE, OUTPUT_BITS, OUTPUT_SIGNED);
          // Set setpoint of the PI controller
          setpoint_l = spd;
          setpoint_r = spd;
        }
      curState = 5;
      break;
  }
}

//! Function of the CMotorCtrl to cperform a stateflow calculation
/*!
   Function performs a stateflow calculation. 
*/
void CMotorCtrl::rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Step the model
  rtObj.step();

  // Indicate task complete
  OverrunFlag = false;
}
