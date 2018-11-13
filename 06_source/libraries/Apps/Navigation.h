/*H**********************************************************************
* FILENAME :        Navigation.h             
*
* DESCRIPTION : Navigation module 
*************************************************************************/

#include <Task_if.h>

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

#endif /*APP_NAVIGATION_H*/
