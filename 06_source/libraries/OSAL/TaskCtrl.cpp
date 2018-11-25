#include "TaskCtrl.h"
#include "Arduino.h"
  
CTaskCtrl::CTaskCtrl()
{
  //do nothing
}
CTaskCtrl::~CTaskCtrl(){
  //do nothing
}

void CTaskCtrl::Init(void)
{
    for (uint8_t i=0; i < MAX_NUM_TASK; i++)
    {
       m_TaskList[i]->Init();
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
    }
    return retVal;
}

void CTaskCtrl::Run(void)
{
    uint8_t i;
    for (i=0; i<MAX_NUM_TASK; i++)
    {
        m_TaskList[i]->Run();
    }
}
