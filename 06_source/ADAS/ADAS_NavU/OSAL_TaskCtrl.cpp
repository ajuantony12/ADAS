/**
  @file OSAL_TaskCtrl.cpp
  @author Aju Antony Jose
  @date January 30, 2019
  @brief Task controller source file

*/
#include "OSAL_TaskCtrl.h"
#include <Arduino.h>

//! Task controller class constructor.
CTaskCtrl::CTaskCtrl()
{
  //do nothing
}

//! Task controller class destructor.
CTaskCtrl::~CTaskCtrl() {
  //do nothing
}

//! Initialization function task controller
void CTaskCtrl::Init(void)
{
  for (uint8_t i = 0; i < MAX_NUM_TASK; i++)
  {
    if (NULL != m_TaskList[i])
    {
      m_TaskList[i]->Init();
    }
  }
}
//! API for registering an application
bool CTaskCtrl::Register(ITask_IF* pTask, uint8_t ID)
{
  bool retVal = false;
  if (MAX_NUM_TASK > ID)
  {
    if ((NULL == m_TaskList[ID]) &&
        (NULL != pTask))
    {
      m_TaskList[ID] = pTask;
    }
  }
  return retVal;
}
//! run all task run function one by one
void CTaskCtrl::Run(void)
{
  uint8_t i;
  for (i = 0; i < MAX_NUM_TASK; i++)
  {
    if (NULL != m_TaskList[i])
    {
      m_TaskList[i]->Run();
    }
  }
}
