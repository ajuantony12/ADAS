/*H**********************************************************************
* FILENAME :        IOManager.h             
*
* DESCRIPTION : IO manaer header
*************************************************************************/
#include "ADAS_Types.h"

#ifndef HAL_IO_MANAGER_H
#define HAL_IO_MANAGER_H

class CIOManager{
public:
  /*
   * Constructor
   */
  CIOManager();
  /*
   * Destructor
   */
  ~CIOManager();
  /*
   * Initialization
   */
  void Init(void);
  /*
   * set Pin
   */
   void ResetPin(uint8_t pinID);
  /*
   * reset Pin
   */
   void SetPin(uint8_t pinID);
  /*
   * toggle Pin
   */
   void TogglePin(uint8_t pinID);
    static const uint8_t Max_IO = 2;
    typedef struct {
        uint8_t ID;
        uint8_t mode : 1;
        uint8_t direction : 1;
        uint8_t reserved : 6;
    }Pin_s;
   static Pin_s m_Ports[Max_IO];
};

#endif /*HAL_IO_MANAGER_H*/
