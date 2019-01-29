/**
* @file HAL_PWM.h
* @author Christoph Jurczyk
* @date January 29, 2019
* @brief This file contains the Hardware Abstraction Layer (HAL) for the PWM outputs
*
*/

#ifndef PWM_16bit_H
#define PWM_16bit_H
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"

class CPWMUnit{
public:
	
   /**
    *
    * \enum    PWMID_e
    *
    * \brief   Enum class for PWM pins
   **/
    typedef enum {
		/// Defintion of first PWM pin (OC1A)
        PWM1=PIN_MTR_L_PWM,
		/// Defintion of second PWM pin (OC1B)
        PWM2=PIN_MTR_R_PWM,
      }PWMID_e;
	  
	  
	/**
    * Constructor of class
    */
    CPWMUnit(PWMID_e ID);
	/**
    * Destructor of class
    */
    ~CPWMUnit();

    // function to initialize 16-bit PWM
    void setupPWM16();


    // function to write PWM duty cycle
    // value from 0 to 65535
    void analogWrite16(uint8_t pin, uint16_t val);

    void writeMOT(uint16_t n);
private:
   PWMID_e m_ID;

    };

#endif
