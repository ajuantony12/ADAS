/*H**********************************************************************
* FILENAME :        Positioning.h             
*
* DESCRIPTION : positioning module 
*************************************************************************/

#include <Task_if.h>
#include <ADAS_Types.h>

#ifndef APP_POSITIONING_H
#define APP_POSITIONING_H

class CPositioning: public ITask_IF
{
public:
	CPositioning();
	~CPositioning();
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

#endif /*APP_POSITIONING_H*/
