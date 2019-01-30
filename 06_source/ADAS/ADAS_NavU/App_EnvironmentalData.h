/**
* @file App_EnvironmentalData.h
* @author Christoph Jurczyk
* @date January 30, 2019
* @brief Application file for environmental data
*
*/

#include "Task_if.h"
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
#include "ADAS_Debug.h"
#include "HAL_ADC.h"

#ifndef APP_ENVIRONMENTALDATA_H
#define APP_ENVIRONMENTALDATA_H

//! Environmental Data Class
class CEnvironmentalData: public ITask_IF
{
  public:
    CEnvironmentalData();
    ~CEnvironmentalData();
    /*
       init
    */
    virtual void Init(void);
    /*
       Run
    */
    virtual void Run(void);
    /*
       Stop
    */
    virtual void Stop(void);

    /*
       Get battery voltage in mV
    */
    virtual uint16_t GetVbat(void);

  private:
    CADC m_ADC;

};

#endif /*APP_ENVIRONMENTALDATA_H*/
