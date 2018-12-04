//
// File: Chart.cpp
//
// Code generated for Simulink model 'Chart'.
//
// Model version                  : 1.24
// Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
// C/C++ source code generated on : Mon Dec  3 13:20:57 2018
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
  real_T tmp;

  // Chart: '<Root>/Chart' incorporates:
  //   Inport: '<Root>/gyro_signal'
  //   Inport: '<Root>/turn'

  switch (rtDW.is_c3_Chart) {
   case IN_Idle:
    if (rtU.turn < 0.0) {
      rtDW.curr_angle = rtU.gyro_signal;
      if (rtDW.curr_angle + rtU.turn > 360.0) {
        rtDW.op = -360.0;
      } else {
        rtDW.op = 0.0;
      }

      rtDW.is_c3_Chart = IN_Turn_Right;

      // Outport: '<Root>/mot' incorporates:
      //   Inport: '<Root>/gyro_signal'
      //   Inport: '<Root>/speed'

      rtY.mot = rtU.speed;
    } else if (rtU.turn > 0.0) {
      rtDW.curr_angle = rtU.gyro_signal;
      rtDW.op = 0.0;
      rtDW.is_c3_Chart = IN_Turn_Left;

      // Outport: '<Root>/mot' incorporates:
      //   Inport: '<Root>/gyro_signal'
      //   Inport: '<Root>/speed'

      rtY.mot = -rtU.speed;
    } else {
      // Outport: '<Root>/mot'
      rtY.mot = 0.0;
    }
    break;

   case IN_Turn_Left:
    tmp = (rtDW.curr_angle + rtU.turn) + rtDW.op;
    if ((real_T)(tmp - 5.0 <= rtU.gyro_signal) <= tmp + 5.0) {
      rtDW.curr_angle = 0.0;
      rtDW.is_c3_Chart = IN_Idle;

      // Outport: '<Root>/mot'
      rtY.mot = 0.0;
    } else {
      // Outport: '<Root>/mot' incorporates:
      //   Inport: '<Root>/speed'

      rtY.mot = -rtU.speed;
    }
    break;

   default:
    tmp = (rtDW.curr_angle + rtU.turn) + rtDW.op;
    if ((real_T)(tmp - 5.0 <= rtU.gyro_signal) <= tmp + 5.0) {
      rtDW.curr_angle = 0.0;
      rtDW.is_c3_Chart = IN_Idle;

      // Outport: '<Root>/mot'
      rtY.mot = 0.0;
    } else {
      // Outport: '<Root>/mot' incorporates:
      //   Inport: '<Root>/speed'

      rtY.mot = rtU.speed;
    }
    break;
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
