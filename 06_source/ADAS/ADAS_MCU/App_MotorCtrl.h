/*H**********************************************************************
* FILENAME :        MotorCtrl.h             
*
* DESCRIPTION : MotorCtrl module 
*************************************************************************/

#include "ADAS_Types.h"
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"
#include "HAL_Encoder.h"
#include "Comm_ICC.h"

#ifndef APP_MOTOR_CTRL_H
#define APP_MOTOR_CTRL_H

class CMotorCtrl
{
public:
	CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, 
	CEncoder& enc1_o, CEncoder& enc2_o, CICCComms& iccComms_o);
	~CMotorCtrl();


    /*
    * init
    */
    void Init(void);
    /*
    * Run
    */
    void Run(void);
    /*
    * Stop
    */
    void Stop(void);

    void startRotation(sint16_t angle);

    void setDistance(sint16_t dist);
    
    void setPISetpoint(uint16_t setpnt);

    void pauseDrive(void);

    void contDrive(void);


private:
    void checkState(void);
    
    void StraightDrive(void);
    
    void MotPI(void);

    void writeMOT(uint16_t n1, uint16_t n2);
    
    static void EncISR_R(void);

    static void EncISR_L(void);

    static void readenc(void* context);

    void rt_OneStep(void);
    ChartModelClass rtObj;
    CIMUUnit& m_imu_o;
    CPWMUnit& m_pwmUnitLeft_o;
    CPWMUnit& m_pwmUnitRight_o;
    CEncoder& m_enc1_o;
    CEncoder& m_enc2_o;
    CICCComms& m_iccComms_o;

    //variables
    int curState, ctrl_side, k, control_area;
    int16_t distance, adapt_r, adapt_l, remindValue;
    boolean pause, speed_set, startBtn, changeDirection;
    uint16_t setpoint_l, spd, setpoint_r, feedback_l, feedback_r, output_r, output_l, lower_b, upper_b, limit_var;
    sint16_t d_way;

};

#endif /*APP_MOTOR_CTRL_H*/
