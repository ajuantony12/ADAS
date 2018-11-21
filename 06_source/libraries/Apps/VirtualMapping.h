/*H**********************************************************************
* FILENAME :        VirtualMapping.h             
*
* DESCRIPTION : Virtual Mapping module 
*************************************************************************/

#include <Task_if.h>
#include <ADAS_Types.h>

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

#endif /*APP_V_MAPPING_H*/
