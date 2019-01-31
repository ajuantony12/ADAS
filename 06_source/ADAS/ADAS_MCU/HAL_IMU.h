/**
* @file HAL_IMU.h
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for IMU
* 
* 
*/


#ifndef HAL_IMU_UNIT_H
#define HAL_IMU_UNIT_H

// ##### Includes #####
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <utility/imumaths.h>
#include "ADAS_Types.h"
/**Library of the IMU Modul*/
#include <Adafruit_BNO055.h>

// ##### Definitions #####
/**
 *
 * \class   CIMUUnit
 *
 * \brief   Class to communicate with IMU Unit
**/
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
   *Return Gyro signal
   */
   uint16_t ReturnGyro(void);

};

#endif /*HAL_IMU_H*/
