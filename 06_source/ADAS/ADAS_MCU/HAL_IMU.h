/*H**********************************************************************
* FILENAME :        IMU.h
*
* DESCRIPTION : Gyro and Acceleration sensor BNO055 header file
*************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "ADAS_Types.h"

#ifndef HAL_IMU_UNIT_H
#define HAL_IMU_UNIT_H

class CIMUUnit{
public:
  /*
   * Constructor
   */
  CIMUUnit();
  /*
   * Destructor
   */
  ~CIMUUnit();
  /*
   * Initialization
   */
  void Init(void);
  /*
   *
   */
   uint16_t ReturnGyro(void);

};

#endif /*HAL_DRIVE_UNIT_H*/
