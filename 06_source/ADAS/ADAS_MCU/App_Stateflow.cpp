/**
* @file App_Stateflow.cpp
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for Stateflow
*
*
* @section Code_generation code_generation
* Code generated for Simulink model 'Chart'.
* Model version                  : 1.40
* Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
* C/C++ source code generated on : Thu Jan 24 11:08:26 2018 
* Embedded hardware selection: Atmel->AVR (8-bit)
* 
*/


// ##### Includes #####
#include "App_Stateflow.h"

// Define constants for Stateflowchart
#define IN_Backward                    ((uint8_T)1U)
#define IN_Forward                     ((uint8_T)2U)
#define IN_Idle                        ((uint8_T)3U)
#define IN_Turn_Left                   ((uint8_T)4U)
#define IN_Turn_Right                  ((uint8_T)5U)

//! Model step function
void ChartModelClass::step()
{
  int16_T qY;
  int16_T qY_0;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T guard3 = false;
  boolean_T guard4 = false;

  guard1 = false;
  guard2 = false;
  guard3 = false;
  guard4 = false;
  switch (rtDW.is_c3_Chart) {
   //State to drive backward
   case IN_Backward:
   //condition to get from backward in idle state
    if ((rtU.turn != 0) || (rtU.dist == 0)) {
      //set local variable to current gyro signal
		rtDW.curr_angle = rtU.gyro_signal;
      // Outport direction right motor
      rtY.dir_r = 0U;

      // Outport direction left motor
      rtY.dir_l = 0U;

      //Next state 
      rtDW.is_c3_Chart = IN_Idle;

      // Outport power motor right off
      rtY.mot_r = 0U;

      // Outport power motor left off
      rtY.mot_l = 0U;
    } else {
      // Outport power motor right on
      rtY.mot_r = 1U;

      // Outport power motor left on
      rtY.mot_l = 1U;

      // Outport direction right motor 
      rtY.dir_r = 1U;

      // Outport direction left motor
      rtY.dir_l = 1U;
    }
    break;
   // State forward
   case IN_Forward:
    //condition to get in from forward to idle state
    if ((rtU.turn != 0) || (rtU.dist == 0)) {
      //set local variable to current gyro signal
		rtDW.curr_angle = rtU.gyro_signal;
      // Outport direction right motor 
      rtY.dir_r = 0U;

      // Outport direction left motor 
      rtY.dir_l = 0U;

      //Next state
      rtDW.is_c3_Chart = IN_Idle;

      // Outport power motor right off
      rtY.mot_r = 0U;

      // Outport power motor left off
      rtY.mot_l = 0U;
    } else {
      // Outport power motor right on
      rtY.mot_r = 1U;

      // Outport power motor right on
      rtY.mot_l = 1U;

      // Outport direction right motor 
      rtY.dir_r = 0U;

      // Outport direction left motor 
      rtY.dir_l = 0U;
    }
    break;

   case IN_Idle:
    if (rtU.turn > 0) {
      //set local variable to current gyro signal
      rtDW.curr_angle = rtU.gyro_signal;
      if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
        qY_0 = MIN_int16_T;
      } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                  - rtDW.curr_angle)) {
        qY_0 = MAX_int16_T;
      } else {
        qY_0 = rtDW.curr_angle + rtU.turn;
      }

      if (qY_0 > 360) {
        rtDW.op = -360;
        guard1 = true;
      } else {
        if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle))
        {
          qY_0 = MIN_int16_T;
        } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                    - rtDW.curr_angle)) {
          qY_0 = MAX_int16_T;
        } else {
          qY_0 = rtDW.curr_angle + rtU.turn;
        }

        if (qY_0 <= 360) {
          rtDW.op = 0;
          guard1 = true;
        } else {
          guard4 = true;
        }
      }
    } else {
      guard4 = true;
    }
    break;

   case IN_Turn_Left:
    if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
      qY_0 = MIN_int16_T;
    } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                - rtDW.curr_angle)) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 = rtDW.curr_angle + rtU.turn;
    }

    if ((qY_0 < 0) && (rtDW.op < MIN_int16_T - qY_0)) {
      qY_0 = MIN_int16_T;
    } else if ((qY_0 > 0) && (rtDW.op > MAX_int16_T - qY_0)) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 += rtDW.op;
    }

    if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
      qY = MIN_int16_T;
    } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                - rtDW.curr_angle)) {
      qY = MAX_int16_T;
    } else {
      qY = rtDW.curr_angle + rtU.turn;
    }

    if ((qY < 0) && (rtDW.op < MIN_int16_T - qY)) {
      qY = MIN_int16_T;
    } else if ((qY > 0) && (rtDW.op > MAX_int16_T - qY)) {
      qY = MAX_int16_T;
    } else {
      qY += rtDW.op;
    }

    if (qY_0 > 32762) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 += 3;
    }

    if (qY < -32763) {
      qY = MIN_int16_T;
    } else {
      qY -= 3;
    }

    if ((rtU.gyro_signal <= qY_0) && (rtU.gyro_signal >= qY)) {
      rtDW.curr_angle = 0;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 0U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 0U;
      rtDW.is_c3_Chart = IN_Idle;

      // Outport: '<Root>/mot_r'
      rtY.mot_r = 0U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 0U;
    } else {
      // Outport: '<Root>/mot_r'
      rtY.mot_r = 1U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 1U;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 0U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 1U;
    }
    break;

   default:
    if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
      qY_0 = MIN_int16_T;
    } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                - rtDW.curr_angle)) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 = rtDW.curr_angle + rtU.turn;
    }

    if ((qY_0 < 0) && (rtDW.op < MIN_int16_T - qY_0)) {
      qY_0 = MIN_int16_T;
    } else if ((qY_0 > 0) && (rtDW.op > MAX_int16_T - qY_0)) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 += rtDW.op;
    }

    if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
      qY = MIN_int16_T;
    } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                - rtDW.curr_angle)) {
      qY = MAX_int16_T;
    } else {
      qY = rtDW.curr_angle + rtU.turn;
    }

    if ((qY < 0) && (rtDW.op < MIN_int16_T - qY)) {
      qY = MIN_int16_T;
    } else if ((qY > 0) && (rtDW.op > MAX_int16_T - qY)) {
      qY = MAX_int16_T;
    } else {
      qY += rtDW.op;
    }

    if (qY_0 > 32762) {
      qY_0 = MAX_int16_T;
    } else {
      qY_0 += 3;
    }

    if (qY < -32763) {
      qY = MIN_int16_T;
    } else {
      qY -= 3;
    }

    if ((rtU.gyro_signal <= qY_0) && (rtU.gyro_signal >= qY)) {
      rtDW.curr_angle = 0;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 0U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 0U;
      rtDW.is_c3_Chart = IN_Idle;

      // Outport: '<Root>/mot_r'
      rtY.mot_r = 0U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 0U;
    } else {
      // Outport: '<Root>/mot_r'
      rtY.mot_r = 1U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 1U;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 1U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 0U;
    }
    break;
  }

  if (guard4) {
    if (rtU.turn < 0) {
      rtDW.curr_angle = rtU.gyro_signal;
      if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
        qY_0 = MIN_int16_T;
      } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                  - rtDW.curr_angle)) {
        qY_0 = MAX_int16_T;
      } else {
        qY_0 = rtDW.curr_angle + rtU.turn;
      }

      if (0 <= qY_0) {
        rtDW.op = 0;
        guard2 = true;
      } else {
        if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle))
        {
          qY_0 = MIN_int16_T;
        } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                    - rtDW.curr_angle)) {
          qY_0 = MAX_int16_T;
        } else {
          qY_0 = rtDW.curr_angle + rtU.turn;
        }

        if (qY_0 < 0) {
          rtDW.op = 360;
          guard2 = true;
        } else {
          guard3 = true;
        }
      }
    } else {
      guard3 = true;
    }
  }

  if (guard3) {
    if ((rtU.turn == 0) && (rtU.dist > 0)) {
      rtDW.is_c3_Chart = IN_Forward;

      // Outport: '<Root>/mot_r'
      rtY.mot_r = 1U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 1U;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 0U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 0U;
    } else if ((rtU.turn == 0) && (rtU.dist < 0)) {
      rtDW.is_c3_Chart = IN_Backward;

      // Outport: '<Root>/mot_r'
      rtY.mot_r = 1U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 1U;

      // Outport: '<Root>/dir_r'
      rtY.dir_r = 1U;

      // Outport: '<Root>/dir_l'
      rtY.dir_l = 1U;
    } else {
      // Outport: '<Root>/mot_r'
      rtY.mot_r = 0U;

      // Outport: '<Root>/mot_l'
      rtY.mot_l = 0U;
    }
  }

  if (guard2) {
    rtDW.is_c3_Chart = IN_Turn_Left;

    // Outport: '<Root>/mot_r'
    rtY.mot_r = 1U;

    // Outport: '<Root>/mot_l'
    rtY.mot_l = 1U;

    // Outport: '<Root>/dir_r'
    rtY.dir_r = 0U;

    // Outport: '<Root>/dir_l'
    rtY.dir_l = 1U;
  }

  if (guard1) {
    rtDW.is_c3_Chart = IN_Turn_Right;

    // Outport: '<Root>/mot_r'
    rtY.mot_r = 1U;

    // Outport: '<Root>/mot_l'
    rtY.mot_l = 1U;

    // Outport: '<Root>/dir_r'
    rtY.dir_r = 1U;

    // Outport: '<Root>/dir_l'
    rtY.dir_l = 0U;
  }

  // End of Chart: '<Root>/Chart'
}

// Model initialize function
void ChartModelClass::initialize()
{
  // Chart: '<Root>/Chart'
  rtDW.is_c3_Chart = IN_Idle;
}

// Constructor
ChartModelClass::ChartModelClass()
{
}

// Destructor
ChartModelClass::~ChartModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
RT_MODEL * ChartModelClass::getRTM()
{
  return (&rtM);
}
