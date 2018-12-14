/*H**********************************************************************
* FILENAME :        PWM.h
*
* DESCRIPTION : PWM signal header file
*************************************************************************/

#ifndef PWM_16bit_H
#define PWM_16bit_H

#include "ADAS_Types.h"

class CPWMUnit{
public:
    typedef enum {
        PWM1,
        PWM2,
      }PWMID_e;
    /*
    * Constructor
    */
    CPWMUnit(PWMID_e ID);
    /*
    * Destructor
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
