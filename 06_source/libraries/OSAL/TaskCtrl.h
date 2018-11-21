/*H**********************************************************************
* FILENAME :        TaskCtrl.h             
*
* DESCRIPTION : Task Control Block file
*************************************************************************/
#include <ADAS_Types.h>
#include <Task_IF.h>

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
    static const uint8_t MaxTask = 5;
    ITask_IF* m_TaskList[MaxTask];
};

#endif /*TASK_CONTROL_BLOCK_H*/
