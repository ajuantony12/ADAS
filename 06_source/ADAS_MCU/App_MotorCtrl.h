/*H**********************************************************************
* FILENAME :        MotorCtrl.h             
*
* DESCRIPTION : MotorCtrl module 
*************************************************************************/

#include "ADAS_Types.h"
#include "Task_if.h"
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "Comm_PLS.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"
#include "HAL_Encoder.h"
#include "Comm_ICC.h"

#ifndef APP_MOTOR_CTRL_H
#define APP_MOTOR_CTRL_H

class CMotorCtrl: public ITask_IF
{
public:
	CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, 
	CEncoder& enc1_o, CEncoder& enc2_o, CICCComms& iccComms_o);
	~CMotorCtrl();


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

    virtual void startRotation(sint16_t angle);

    virtual void setDistance(sint16_t dist);

    virtual void pauseDrive(void);

    virtual void contDrive(void);


private:
    void checkState(void);
    
    void StraightDrive(void);
    
    void MotPI(void);

    void writeMOT(uint16_t n1, uint16_t n2);
    
    static void EncISR_R(void);

    static void EncISR_L(void);

    static void readenc(void* context);
  
#ifdef ADAS_DEBUG
    void getUserInput(void);

    void printValues(void);
#endif

    void rt_OneStep(void);
    ChartModelClass rtObj;
    CIMUUnit& m_imu_o;
    CPWMUnit& m_pwmUnitLeft_o;
    CPWMUnit& m_pwmUnitRight_o;
    CEncoder& m_enc1_o;
    CEncoder& m_enc2_o;
    CICCComms& m_iccComms_o;
};

#endif /*APP_MOTOR_CTRL_H*/
