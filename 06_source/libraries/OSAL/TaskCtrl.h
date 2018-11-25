/*H**********************************************************************
* FILENAME :        TaskCtrl.h             
*
* DESCRIPTION : Task Control Block file
*************************************************************************/
#include <ADAS_Types.h>
#include <Task_IF.h>
#include <ADAS_Cfg.h>

#ifndef TASK_CONTROL_BLOCK_H
#define TASK_CONTROL_BLOCK_H

class CTaskCtrl
{
public:
    CTaskCtrl();
    ~CTaskCtrl();
  /*
   * init
   */
   void Init(void);
  /*
   * Register Task to controller
   */
  bool Register(ITask_IF* pTask, uint8_t ID);
  /*
   * Run
   */
  void Run(void);
private:
    ITask_IF* m_TaskList[MAX_NUM_TASK];
};

#endif /*TASK_CONTROL_BLOCK_H*/
