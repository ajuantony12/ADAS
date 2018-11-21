/*H**********************************************************************
* FILENAME :        UserInterface.h             
*
* DESCRIPTION : user interface module 
*************************************************************************/

#include <Task_if.h>
#include <ADAS_Types.h>

#ifndef APP_UI_H
#define APP_UI_H

class CUser_IF: public ITask_IF
{
public:
	CUser_IF();
	~CUser_IF();
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

#endif /*APP_UI_H*/
