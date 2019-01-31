/**
* @file App_MotorCtrl.h
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Header file for motor control unit
*
*
* @section Code_generation code_generation
* Code generated for Simulink model 'Chart'.
* Model version                  : 1.37
* Simulink Coder version         : 8.14 (R2018a) 06-Feb-2018
* C/C++ source code generated on : Mon Dec 10 09:08:26 2018 
* Embedded hardware selection: Atmel->AVR (8-bit)
* 
* 
* 
* 
*/

// ##### Includes #####
#include "ADAS_Types.h"
#include "HAL_IMU.h"
#include "HAL_PWM.h"
#include "App_Stateflow.h"
#include "App_Stateflowtypes.h"
#include "HAL_Encoder.h"
#include "Comm_ICC.h"


#ifndef APP_MOTOR_CTRL_H
#define APP_MOTOR_CTRL_H

/**
 *
 * \class CMotorCtl
 *
 * \brief   Class declaration for motor control
**/
class CMotorCtrl
{
public:
	CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, CICCComms& iccComms_o);
	~CMotorCtrl();


    /// initialize the motor control
    void Init(void);

    /// run the motor control
    void Run(void);
    
    void Stop(void);

    /// API function to start a rotation with a desired angle
    void startRotation(sint16_t angle);

    /// API function to drive a distance forward and backwards
    void setDistance(sint16_t dist);

    /// API function to change the speed
    void setPISetpoint(uint16_t setpnt);

    /// API to pause the current action of the motor control
    void pauseDrive(void);

    /// API function to continue with the action of the motor control after pausing it
    void contDrive(void);

private:
    /// Check for the current state of the motor control stateflow 
    void checkState(void);

    /// Position control function
    void StraightDrive(void);

    /// PI control function
    void MotPI(void);

    /// Function to set a PWM signal to the motors
    void writeMOT(uint16_t n1, uint16_t n2);

    /// Interrupt Service Routine to count the peaks of the right encoder
    static void EncISR_R(void);

    /// Interrupt Service Routine to count the peaks of the left encoder
    static void EncISR_L(void);

    /// Function to read out the encoder peaks
    static void readenc(void* context);

    /// Function to perform a stateflow calculation 
    void rt_OneStep(void);

    ChartModelClass rtObj;
    CIMUUnit& m_imu_o;
    CPWMUnit& m_pwmUnitLeft_o;
    CPWMUnit& m_pwmUnitRight_o;
    CICCComms& m_iccComms_o;

    /// Declarations of necessary variables
    int curState, ctrl_side, k, control_area;
    int16_t distance, adapt_r, adapt_l, remindValue;
    boolean pause, speed_set, startBtn, changeDirection;
    uint16_t setpoint_l, spd, setpoint_r, feedback_l, feedback_r, output_r, output_l, lower_b, upper_b, limit_var;
    sint16_t d_way;

};

#endif /*APP_MOTOR_CTRL_H*/
