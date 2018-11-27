//
// File: Chart.cpp
//
// Code generated for Simulink model 'Chart'.
//
// Model version                  : 1.0
// Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
// C/C++ source code generated on : Mon Nov 26 14:12:35 2018
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
#define IN_S1                          ((uint8_T)1U)
#define IN_S2                          ((uint8_T)2U)

// Model step function
void ChartModelClass::step()
{
  // Chart: '<Root>/Chart' incorporates:
  //   Inport: '<Root>/I1'
  //   Inport: '<Root>/I2'

  if (rtDW.is_active_c3_Chart == 0U) {
    rtDW.is_active_c3_Chart = 1U;
    rtDW.is_c3_Chart = IN_S1;
  } else if (rtDW.is_c3_Chart == IN_S1) {
    if (!rtU.I1) {
      rtDW.is_c3_Chart = IN_S2;
    } else {
      // Outport: '<Root>/Out'
      rtY.Out = true;
    }
  } else if (!rtU.I2) {
    rtDW.is_c3_Chart = IN_S1;
  } else {
    // Outport: '<Root>/Out'
    rtY.Out = false;
  }

  // End of Chart: '<Root>/Chart'
}

// Model initialize function
void ChartModelClass::initialize()
{
  // (no initialization code required)
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
