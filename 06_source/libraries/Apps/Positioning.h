/*H**********************************************************************
* FILENAME :        Positioning.h             
*
* DESCRIPTION : positioning module 
*************************************************************************/

#include <Task_if.h>

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

#endif /*APP_POSITIONING_H*/
