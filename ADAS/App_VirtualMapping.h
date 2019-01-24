/*H**********************************************************************
* FILENAME :        VirtualMapping.h             
*
* DESCRIPTION : Virtual Mapping module 
*************************************************************************/

#include "Task_if.h"
#include "ADAS_Types.h"
#include "Comm_PLS.h"

#ifndef APP_V_MAPPING_H
#define APP_V_MAPPING_H

class CVMapping: public ITask_IF
{
public:
	CVMapping(CPLSComms& plsComms, uint8_t activeCheckInt);
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
private:
    CPLSComms&   m_plsComms;
    uint8_t m_RunCount;
    const uint8_t m_ActiveCheckInterval;
};

#endif /*APP_V_MAPPING_H*/
