/*H**********************************************************************
* FILENAME :        MotorCtrl.h             
*
* DESCRIPTION : MotorCtrl module 
*************************************************************************/

#include "ADAS_Types.h"
#include <Task_if.h>
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"

#ifndef APP_MOTOR_CTRL_H
#define APP_MOTOR_CTRL_H

class CMotorCtrl: public ITask_IF
{
public:
	CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o);
	~CMotorCtrl();

	// Initialize stateflow object
    ChartModelClass rtObj;
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

    void checkState(void);

    void writeMOT(uint16_t n1, uint16_t n2);

    void getUserInput(void);

    void printValues(void);

    void rt_OneStep(void);
};

#endif /*APP_MOTOR_CTRL_H*/
