/**
* @file App_Stateflow.h
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for Stateflow
*
*
* @section Code_generation code_generation
* Code generated for Simulink model 'Chart'.
* Model version                  : 1.37
* Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
* C/C++ source code generated on : Mon Dec 10 09:08:26 2018 
* Embedded hardware selection: Atmel->AVR (8-bit)
* 
* 
* 
* 
*/

#ifndef RTW_HEADER_Chart_h_
#define RTW_HEADER_Chart_h_
#ifndef Chart_COMMON_INCLUDES_
# define Chart_COMMON_INCLUDES_

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif


// ##### Includes #####
#include "App_Stateflowtypes.h"
#endif                                


// ##### Definitions #####
/**
 *
 * \struct tag_RTM RT_MODEL
 *
 * \brief   Forward declaration for rtModel
**/
typedef struct tag_RTM RT_MODEL;

/**
 *
 * \struct DW
 *
 * \brief   Local signals for Stateflow
**/
typedef struct {
  ///Definition of local variable for current angle
  int16_T curr_angle;                  
  ///Definiton of local variable operation point
  int16_T op;
  ///Definiton of local variable to handle stateflow                         
  uint8_T is_c3_Chart;                 
} DW;

/**
 *
 * \struct ExtU
 *
 * \brief   External inputs for Stateflow
**/
typedef struct {
  ///Definition input for the gyro signal 
  int16_T gyro_signal;                 
  ///Definition input for the turn set 
  int16_T turn;                        
  ///Definition input for the distance set 
  int16_T dist;                        
} ExtU;

/**
 *
 * \struct ExtY
 *
 * \brief   External outputs for Stateflow
**/ 
typedef struct {
  ///Definition output power motor right 
  uint8_T mot_r;                       
  ///Definition output direction motor left 
  uint8_T dir_l;                       
  ///Definition output direction motor right
  uint8_T dir_r;                       
  ///Definition output power motor right 
  uint8_T mot_l;                       
} ExtY;


/**
 *
 * \struct tag_RTM
 *
 * \brief   Real-time Model Data Structure for Stateflow
**/
struct tag_RTM {
  const char_T * volatile errorStatus;
};



//! Class declaration of the chart model
class ChartModelClass {
  /// public data and function members
 public:
 /// Block signals and states
  DW rtDW;
  /// External inputs
  ExtU rtU;

  /// External outputs
  ExtY rtY;

  /// model initialize function
  void initialize();

  /// model step function
  void step();

  /// Constructor
  ChartModelClass();

  /// Destructor
  ~ChartModelClass();

  /// Real-Time Model get method
  RT_MODEL * getRTM();

  /// private data and function members
 private:
  

  /// Real-Time Model
  RT_MODEL rtM;
};



#endif                                 /*APP_Stateflow_H*/
