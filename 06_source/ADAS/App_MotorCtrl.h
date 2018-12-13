/*H**********************************************************************
* FILENAME :        MotorCtrl.h             
*
* DESCRIPTION : MotorCtrl module 
*************************************************************************/

#include "ADAS_Types.h"
#include "Task_if.h"

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
   virtual void Init(void);
  /*
   * Run
   */
  virtual void Run(void);
  /*
   * Stop
   */
  virtual void Stop(void);
  
};

#endif /*APP_MOTOR_CTRL_H*/
