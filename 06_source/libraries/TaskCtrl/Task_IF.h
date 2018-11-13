/*H**********************************************************************
* FILENAME :        Task_IF.h             
*
* DESCRIPTION : Task interface will be 
				used by the task controller block for handfling  task
*************************************************************************/

#ifndef TASK_CONTROL_TASK_IF
#define TASK_CONTROL_TASK_IF

class ITask_IF{
public:
  /*
   * Initialization
   */
  void Init(void)=0;
  /*
   * Run
   */
  void Run(void)=0;
  /*
   * Stop
   */
  void Stop(void)=0;
  
};

#endif /*TASK_CONTROL_TASK_IF*/
