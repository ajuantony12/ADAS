/*H**********************************************************************
* FILENAME :        Task_IF.h             
*
* DESCRIPTION : Task interface will be 
				used by the task controller block for handfling  task
*************************************************************************/
#include "ADAS_Types.h"

#ifndef TASK_CONTROL_TASK_IF
#define TASK_CONTROL_TASK_IF

class ITask_IF{
public:
  /*
   * Initialization
   */
  virtual void Init(void)=0;
  /*
   * Run
   */
  virtual void Run(void)=0;
  /*
   * Stop
   */
  virtual void Stop(void)=0;
  
};

#endif /*TASK_CONTROL_TASK_IF*/
