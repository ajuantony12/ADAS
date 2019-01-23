/*H**********************************************************************
* FILENAME :        PWM.h
*
* DESCRIPTION : PWM signal header file
*************************************************************************/

#ifndef PWM_16bit_H
#define PWM_16bit_H

class CPWMUnit{
public:
    typedef enum {
        PWM1=11,
        PWM2=12,
        //PWM1=PIN_MTR_L_PWM,
        //PWM2=PIN_MTR_R_PWM,
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
    void setupPWM10();


    // function to write PWM duty cycle
    // value from 0 to 1024
    void analogWrite10(uint8_t pin, uint16_t val);

    void writeMOT(uint16_t n);
private:
   PWMID_e m_ID;

    };

#endif
