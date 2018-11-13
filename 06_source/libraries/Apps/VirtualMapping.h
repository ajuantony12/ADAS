/*H**********************************************************************
* FILENAME :        VirtualMapping.h             
*
* DESCRIPTION : Virtual Mapping module 
*************************************************************************/

#include <Task_if.h>

#ifndef APP_V_MAPPING_H
#define APP_V_MAPPING_H

class CVMapping: public ITask_IF
{
public:
	CVMapping();
	~CVMapping();
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

#endif /*APP_V_MAPPING_H*/
