/**
  @file OSAL_TaskCtrl.h
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Task controller header file

*/
#include "ADAS_Types.h"
#include "Task_IF.h"
#include "ADAS_Cfg.h"

#ifndef TASK_CONTROL_BLOCK_H
#define TASK_CONTROL_BLOCK_H
//! Task scheduler class
/*!
  implements scheduling functionality for ADAS
*/
class CTaskCtrl
{
  public:
    //! Task controller class constructor.
    CTaskCtrl();
    //! Task controller class destructor.
    ~CTaskCtrl();
    //! Initialization function task controller
    /*!
      \brief Initialize all application.
    */
    void Init(void);
    //! API for registering an application
    /*!
      \param[in] pTask pointer to application
      \param[in] ID priorityID of application
      \return true if successful
    */
    bool Register(ITask_IF* pTask, uint8_t ID);
    //! run all task run function one by one
    /*!
      \brief run all run of applications one by one
    */
    void Run(void);
  private:
    ITask_IF* m_TaskList[MAX_NUM_TASK];     /*!<  list application to be scheduled*/
};

#endif /*TASK_CONTROL_BLOCK_H*/
