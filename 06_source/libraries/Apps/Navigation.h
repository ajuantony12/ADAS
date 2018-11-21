/*H**********************************************************************
* FILENAME :        Navigation.h             
*
* DESCRIPTION : Navigation module 
*************************************************************************/

#include <Task_if.h>
#include <ADAS_Types.h>

#ifndef APP_NAVIGATION_H
#define APP_NAVIGATION_H

class CNavigation: public ITask_IF
{
public:
	CNavigation();
	~CNavigation();
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

#endif /*APP_NAVIGATION_H*/
