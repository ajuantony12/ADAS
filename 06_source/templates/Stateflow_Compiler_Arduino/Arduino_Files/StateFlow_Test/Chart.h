//
// File: Chart.h
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
#ifndef RTW_HEADER_Chart_h_
#define RTW_HEADER_Chart_h_
#ifndef Chart_COMMON_INCLUDES_
# define Chart_COMMON_INCLUDES_
#include "rtwtypes.h"
//#include "rtw_continuous.h"
//#include "rtw_solver.h"
#endif                                 // Chart_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Forward declaration for rtModel
typedef struct tag_RTM RT_MODEL;

// Block signals and states (default storage) for system '<Root>'
typedef struct {
  uint8_T is_active_c3_Chart;          // '<Root>/Chart'
  uint8_T is_c3_Chart;                 // '<Root>/Chart'
} DW;

// External inputs (root inport signals with default storage)
typedef struct {
  boolean_T I1;                        // '<Root>/I1'
  boolean_T I2;                        // '<Root>/I2'
} ExtU;

// External outputs (root outports fed by signals with default storage)
typedef struct {
  boolean_T Out;                       // '<Root>/Out'
} ExtY;

// Real-time Model Data Structure
struct tag_RTM {
  const char_T * volatile errorStatus;
};

// Class declaration for model Chart
class ChartModelClass {
  // public data and function members
 public:
  // External inputs
  ExtU rtU;

  // External outputs
  ExtY rtY;

  // model initialize function
  void initialize();

  // model step function
  void step();

  // Constructor
  ChartModelClass();

  // Destructor
  ~ChartModelClass();

  // Real-Time Model get method
  RT_MODEL * getRTM();

  // private data and function members
 private:
  // Block signals and states
  DW rtDW;

  // Real-Time Model
  RT_MODEL rtM;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Note that this particular code originates from a subsystem build,
//  and has its own system numbers different from the parent model.
//  Refer to the system hierarchy for this subsystem below, and use the
//  MATLAB hilite_system command to trace the generated code back
//  to the parent model.  For example,
//
//  hilite_system('untitled/Chart')    - opens subsystem untitled/Chart
//  hilite_system('untitled/Chart/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'untitled'
//  '<S1>'   : 'untitled/Chart'

#endif                                 // RTW_HEADER_Chart_h_

//
// File trailer for generated code.
//
// [EOF]
//
