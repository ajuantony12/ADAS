/**
  @file Task_IF.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Task controller interface header

*/
#include "ADAS_Types.h"

#ifndef TASK_CONTROL_TASK_IF
#define TASK_CONTROL_TASK_IF
//! Task scheduler class interface class
/*!
  Abstract class which should be realized by all application
*/
class ITask_IF {
  public:
    //! Initialization pure virtual method
    virtual void Init(void) = 0;
    //! run pure virtual method
    virtual void Run(void) = 0;
    //! stop pure virtual method
    virtual void Stop(void) = 0;

};

#endif /*TASK_CONTROL_TASK_IF*/
