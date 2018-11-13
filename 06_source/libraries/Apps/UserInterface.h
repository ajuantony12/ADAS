/*H**********************************************************************
* FILENAME :        UserInterface.h             
*
* DESCRIPTION : user interface module 
*************************************************************************/

#include <Task_if.h>

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

#endif /*APP_UI_H*/
