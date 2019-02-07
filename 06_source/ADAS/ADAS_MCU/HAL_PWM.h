/**
* @file HAL_PWM.h
* @author Christoph Jurczyk
* @date January 29, 2019
* @brief Hardware Abstraction Layer (HAL) for the 10-bit PWM outputs
*
*/
​
#ifndef HAL_PWM_H
#define HAL_PWM_H
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"
​
//! Hardware Abstraction Layer (HAL) PWM class
class CPWMUnit{
public:

  /**
  *
  * \enum  PWMID_e
  *
  * \brief  Enum class for PWM pins
  **/
  typedef enum {
		/// Definition of first PWM pin (OC1A)
    PWM1=PIN_MTR_L_PWM,
		/// Definition of second PWM pin (OC1B)
    PWM2=PIN_MTR_R_PWM,
   }PWMID_e;
    /*
    * Constructor
    */
    CPWMUnit(PWMID_e ID);
    /*
    * Destructor
    */
    ~CPWMUnit();

    // function to initialize 10-bit PWM
    void setupPWM10();

    /*
    * Function to write a PWM duty cycle on the specific pin
    */
    void writeDuty(uint16_t n);
private:
   PWMID_e m_ID;
    // function to write PWM duty cycle
    // value from 0 to 65535
    void analogWrite10(uint8_t pin, uint16_t val);
    };

#endif
