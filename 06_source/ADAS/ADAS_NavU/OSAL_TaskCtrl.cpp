#include "OSAL_TaskCtrl.h"
#include "ADAS_Debug.h"
#include <Arduino.h>

CTaskCtrl::CTaskCtrl()
{
  //do nothing
}
CTaskCtrl::~CTaskCtrl() {
  //do nothing
}

void CTaskCtrl::Init(void)
{
  for (uint8_t i = 0; i < MAX_NUM_TASK; i++)
  {
    if (NULL != m_TaskList[i])
    {
      DPRINT(i, DEC);
      DPRINTLN(" Intializing");
      m_TaskList[i]->Init();
    }
  }
}

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
#ifdef ADAS_DEBUG
    else
    {
      DPRINTLN("failed to register");
    }
#endif
  }
#ifdef ADAS_DEBUG
  else
  {
    DPRINTLN("Wrong ID");
  }
#endif
  return retVal;
}

void CTaskCtrl::Run(void)
{
  uint8_t i;
  for (i = 0; i < MAX_NUM_TASK; i++)
  {
    if (NULL != m_TaskList[i])
    {
      DPRINT(i, DEC);
      DPRINTLN(" Running");
      m_TaskList[i]->Run();
    }
  }
}
