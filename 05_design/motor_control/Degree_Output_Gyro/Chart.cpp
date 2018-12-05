//
// File: Chart.cpp
//
// Code generated for Simulink model 'Chart'.
//
// Model version                  : 1.33
// Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
// C/C++ source code generated on : Wed Dec  5 14:26:11 2018
//
// Target selection: ert.tlc
// Embedded hardware selection: Atmel->AVR (8-bit)
// Code generation objectives:
//    1. Execution efficiency
//    2. RAM efficiency
// Validation result: Not run
//
#include "Chart.h"

// Named constants for Chart: '<Root>/Chart'
#define IN_Idle                        ((uint8_T)1U)
#define IN_Turn_Left                   ((uint8_T)2U)
#define IN_Turn_Right                  ((uint8_T)3U)
// Model step function
void ChartModelClass::step()
{
  int16_T qY;
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T guard3 = false;
  boolean_T guard4 = false;

  // Chart: '<Root>/Chart' incorporates:
  //   Inport: '<Root>/gyro_signal'
  //   Inport: '<Root>/turn'

  guard1 = false;
  guard2 = false;
  guard3 = false;
  guard4 = false;
  switch (rtDW.is_c3_Chart) {
   case IN_Idle:
    if (rtU.turn < 0) {
      rtDW.curr_angle = rtU.gyro_signal;
      if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
        qY = MIN_int16_T;
      } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                  - rtDW.curr_angle)) {
        qY = MAX_int16_T;
      } else {
        qY = rtDW.curr_angle + rtU.turn;
      }

      if (qY > 360) {
        rtDW.op = -360;
        guard1 = true;
      } else {
        if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle))
        {
          qY = MIN_int16_T;
        } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                    - rtDW.curr_angle)) {
          qY = MAX_int16_T;
        } else {
          qY = rtDW.curr_angle + rtU.turn;
        }

        if (qY <= 360) {
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
      qY = MIN_int16_T;
    } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                - rtDW.curr_angle)) {
      qY = MAX_int16_T;
    } else {
      qY = rtDW.curr_angle + rtU.turn;
    }
    Serial.print("qY=");
    Serial.println(qY);

    if ((qY < 0) && (rtDW.op < MIN_int16_T - qY)) {
      qY = MIN_int16_T;
    } else if ((qY > 0) && (rtDW.op > MAX_int16_T - qY)) {
      qY = MAX_int16_T;
    } else {
      qY += rtDW.op;
    }
    Serial.print("qY=");
    Serial.println(qY);
    if (rtU.gyro_signal == qY) {
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

    if (qY == rtU.gyro_signal) {
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
    if (rtU.turn > 0) {
      rtDW.curr_angle = rtU.gyro_signal;
      if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle)) {
        qY = MIN_int16_T;
      } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                  - rtDW.curr_angle)) {
        qY = MAX_int16_T;
      } else {
        qY = rtDW.curr_angle + rtU.turn;
      }

      if (0 <= qY) {
        rtDW.op = 0;
        guard2 = true;
      } else {
        if ((rtDW.curr_angle < 0) && (rtU.turn < MIN_int16_T - rtDW.curr_angle))
        {
          qY = MIN_int16_T;
        } else if ((rtDW.curr_angle > 0) && (rtU.turn > MAX_int16_T
                    - rtDW.curr_angle)) {
          qY = MAX_int16_T;
        } else {
          qY = rtDW.curr_angle + rtU.turn;
        }

        if (qY < 0) {
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
    // Outport: '<Root>/mot_r'
    rtY.mot_r = 0U;

    // Outport: '<Root>/mot_l'
    rtY.mot_l = 0U;
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

//
// File trailer for generated code.
//
// [EOF]
//
