/*H**********************************************************************
* FILENAME :        DriveUnit.h             
*
* DESCRIPTION : Drive unit header file
*************************************************************************/
#include "ADAS_Types.h"

#ifndef HAL_DRIVE_UNIT_H
#define HAL_DRIVE_UNIT_H

class CDriveUnit{
public:
  typedef enum {
    Drive1,
    Drive2,
  }DriveID_e;
  /*
   * Constructor
   */
  CDriveUnit(DriveID_e ID);
  /*
   * Destructor
   */
  ~CDriveUnit();
  /*
   * Initialization
   */
  void Init(void);
  /*
   * Set speed
   */
   bool SetSpeed(uint8_t speed);
 private:
   DriveID_e m_ID;
  
};

#endif /*HAL_DRIVE_UNIT_H*/
