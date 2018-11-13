/*H**********************************************************************
* FILENAME :        ADC_IF.h             
*
* DESCRIPTION : ADC Hardware interface
*************************************************************************/

#ifndef HAL_ADC_IF
#define HAL_ADC_IF

class CADC_IF{
public:
  /*
   * Constructor
   */
  CADC_IF();
  /*
   * Destructor
   */
  ~CADC_IF();
  /*
   * Initialization
   */
  void Init();
  /*
   * Send Data
   */
   unsigned short ReadADCValue(void);
  
};

#endif /*HAL_ADC_IF*/
