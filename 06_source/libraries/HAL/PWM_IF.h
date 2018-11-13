/*H**********************************************************************
* FILENAME :        PWM_IF.h             
*
* DESCRIPTION : PWM interface file
*************************************************************************/

#ifndef HAL_PWM_IF
#define HAL_PWM_IF

class CPWMIF{
public:
  typedef enum {
    PWM1,
    PWM2,
  }PWMID_e;
  /*
   * Constructor
   */
  CPWMIF(PWMID_e ID);
  /*
   * Destructor
   */
  ~CPWMIF();
  /*
   * Initialization
   */
  void Init(void);
  /*
   * Send Data
   */
   bool setDutyCycle(unsigned char duty);
 private:
   PWMID_e m_ID;
  
};

#endif /*HAL_PWM_IF*/
