/*H**********************************************************************
* FILENAME :        IMU.cpp
*
* DESCRIPTION : Gyro and Acceleration sensor
*************************************************************************/

#include "HAL_IMU.h"
#include <Arduino.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

CIMUUnit::CIMUUnit(){
  //do nothing
}
CIMUUnit::~CIMUUnit(){

}

void CIMUUnit::Init(void){
    DPRINTLN("Orientation Sensor Test Started..."); Serial.println("");
	/* Initialise the sensor */
      if (!bno.begin())
      {
        /* There was a problem detecting the BNO055 ... check your connections */
        DPRINTLN("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
      }

        bno.setExtCrystalUse(true);
}

 uint16_t CIMUUnit::ReturnGyro(void){
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

      return (int16_t)euler.x();
 }