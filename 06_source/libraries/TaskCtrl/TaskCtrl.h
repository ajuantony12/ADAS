/*H**********************************************************************
* FILENAME :        TaskCtrl.h             
*
* DESCRIPTION : Task Control Block file
*************************************************************************/

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
   void init(voit);
  /*
   * Register Task to controller
   */
  void Register(ITask_IF& iTask);
  /*
   * Run
   */
  void Run(void);
  
};

#endif /*TASK_CONTROL_BLOCK_H*/
