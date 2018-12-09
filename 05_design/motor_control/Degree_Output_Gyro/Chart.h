//
// File: Chart.h
//
// Code generated for Simulink model 'Chart'.
//
// Model version                  : 1.35
// Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
// C/C++ source code generated on : Thu Dec  6 11:59:00 2018
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
  int16_T curr_angle;                  // '<Root>/Chart'
  int16_T op;                          // '<Root>/Chart'
  uint8_T is_c3_Chart;                 // '<Root>/Chart'
} DW;

// External inputs (root inport signals with default storage)
typedef struct {
  int16_T gyro_signal;                 // '<Root>/gyro_signal'
  int16_T turn;                        // '<Root>/turn'
} ExtU;

// External outputs (root outports fed by signals with default storage)
typedef struct {
  uint8_T mot_r;                       // '<Root>/mot_r'
  uint8_T dir_l;                       // '<Root>/dir_l'
  uint8_T dir_r;                       // '<Root>/dir_r'
  uint8_T mot_l;                       // '<Root>/mot_l'
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
//  hilite_system('Motor_control_simulation/Control/Chart')    - opens subsystem Motor_control_simulation/Control/Chart
//  hilite_system('Motor_control_simulation/Control/Chart/Kp') - opens and selects block Kp
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Motor_control_simulation/Control'
//  '<S1>'   : 'Motor_control_simulation/Control/Chart'

#endif                                 // RTW_HEADER_Chart_h_

//
// File trailer for generated code.
//
// [EOF]
//
