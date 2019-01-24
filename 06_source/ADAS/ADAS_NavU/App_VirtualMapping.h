/*H**********************************************************************
* FILENAME :        VirtualMapping.h             
*
* DESCRIPTION : Virtual Mapping module 
*************************************************************************/

#include "Task_if.h"
#include "ADAS_Types.h"
#include "Comm_PLS.h"
#include "App_Navigation.h"

#ifndef APP_V_MAPPING_H
#define APP_V_MAPPING_H

class CVMapping: public ITask_IF
{
public:
	CVMapping(CNavigation& NAV, CPLSComms& plsComms, uint8_t activeCheckInt);
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
    uint16_t OrganizeData(uint16_t* data, const uint16_t len);
    bool searchforPFBreach(uint16_t* data, uint16_t len, bool pfStatus);
    void calculateWallInfo(uint16_t* data, uint16_t len, bool wfStatus, bool pfStatus);
    sint16_t CordicATan(sint32_t y, sint32_t x);
    
    CNavigation& m_NAV;
    CPLSComms&   m_plsComms;
    uint8_t m_RunCount;
    const uint8_t m_ActiveCheckInterval;
};

#endif /*APP_V_MAPPING_H*/
