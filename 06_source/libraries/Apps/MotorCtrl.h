/*H**********************************************************************
* FILENAME :        MotorCtrl.h             
*
* DESCRIPTION : MotorCtrl module 
*************************************************************************/

#include <Task_if.h>

#ifndef APP_MOTOR_CTRL_H
#define APP_MOTOR_CTRL_H

class CMotorCtrl: public ITask_IF
{
public:
	CMotorCtrl();
	~CMotorCtrl();
  /*
   * init
   */
   void init(void);
  /*
   * Run
   */
  void Run(void);
  /*
   * Stop
   */
  void Stop(void);
  
};

#endif /*APP_MOTOR_CTRL_H*/
