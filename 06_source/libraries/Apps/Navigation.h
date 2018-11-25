/*H**********************************************************************
* FILENAME :        Navigation.h             
*
* DESCRIPTION : Navigation module 
*************************************************************************/

#include <Task_if.h>
#include <ADAS_Types.h>
#include <ADAS_Cfg.h>
#include <PLSComms.h>

#ifndef APP_NAVIGATION_H
#define APP_NAVIGATION_H

class CNavigation: public ITask_IF
{
public:
	CNavigation(CPLSComms& plsComms);
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
private:
    CPLSComms&   m_plsComms;
};

#endif /*APP_NAVIGATION_H*/
