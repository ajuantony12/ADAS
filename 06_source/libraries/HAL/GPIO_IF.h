/*H**********************************************************************
* FILENAME :        GPIO_IF.h             
*
* DESCRIPTION : GPIO Hardware interface
*************************************************************************/

#ifndef HAL_GPIO_IF
#define HAL_GPIO_IF

class CGPIO_IF{
public:
  /*
   * Constructor
   */
  CGPIO_IF(PortNum_e ID);
  /*
   * Destructor
   */
  ~CGPIO_IF();
  /*
   * Initialization
   */
  void Init(void);
  /*
   * Send Data
   */
   bool SetPinMode(unsigned char pinID, unsigned char mode);
 private:
   PortNum_e m_PortID;
  
};

#endif /*HAL_GPIO_IF*/
