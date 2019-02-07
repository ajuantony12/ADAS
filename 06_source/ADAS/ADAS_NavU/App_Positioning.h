/**
  @file App_Positioning.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief template header file for positioning application

*/

#include "Task_if.h"
#include "ADAS_Types.h"

#ifndef APP_POSITIONING_H
#define APP_POSITIONING_H

/*!
  Template class positioning
  contains only basic functionality required by task controller
*/
class CPositioning: public ITask_IF
{
  public:
    //! position app class constructor.
    CPositioning();
    //! position app class destructor.
    ~CPositioning();
    //! Initialization function of Positioning Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for initializing applications
    */
    virtual void Init(void);
    //! Run function of Positioning Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for running functionality
    */
    virtual void Run(void);
    //! Stop/shutdown function of Positioning Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for shutdown
    */
    virtual void Stop(void);

};

#endif /*APP_POSITIONING_H*/
