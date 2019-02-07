/*H**********************************************************************
* FILENAME :        PWM.h
*
* DESCRIPTION : PWM signal header file
*************************************************************************/

#ifndef PWM_16bit_H
#define PWM_16bit_H
#include "ADAS_Types.h"
#include "ADAS_Cfg.h"

class CPWMUnit{
public:
    typedef enum {
        PWM1=PIN_MTR_L_PWM,
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
