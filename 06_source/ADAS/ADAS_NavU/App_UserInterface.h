/**
  @file App_UserInterface.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief template header file for uner interface application

*/

#include "Task_if.h"
#include "ADAS_Types.h"

#ifndef APP_UI_H
#define APP_UI_H

/*!
  Template class user interface
  contains only basic functionality required by task controller
*/
class CUser_IF: public ITask_IF
{
  public:
    //! user interface app class constructor.
    CUser_IF();
    //! user interface app class destructor.
    ~CUser_IF();
    //! Initialization function of user interface  Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for initializing applications
    */
    virtual void Init(void);
    //! Run function of user interface Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for running functionality
    */
    virtual void Run(void);
    //! Stop/shutdown function of user interface Application
    /*!
      \brief Realization of pure virtual function task_IF
             Called by task controller for shutdown
    */
    virtual void Stop(void);

};

#endif /*APP_UI_H*/
