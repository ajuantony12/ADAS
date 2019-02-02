/**
* @file HAL_IMU.cpp
* @author Hannes Bähr, Juliane Müller
* @date January 31, 2019
* @brief Application file for IMU
* 
* 
*/


// ##### Includes #####
#include "HAL_IMU.h"
#include "ADAS_Debug.h"
#include <Arduino.h>



Adafruit_BNO055 bno = Adafruit_BNO055();


//! Constructor of CIMUUnit
CIMUUnit::CIMUUnit(){
  //do nothing
}

//! Destructor of CIMUUnit
CIMUUnit::~CIMUUnit(){

}
//! Intitialization function of IMU Unit
/*!
  Function checks if the sensor ready to communicate
*/
void CIMUUnit::Init(void){
    DPRINTLN("Orientation Sensor Test Started..."); 
    DPRINTLN("");
	    //Initialize the sensor
      if (!bno.begin())
      {
        //Error output
        DPRINTLN("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
      }

        bno.setExtCrystalUse(true);
}
 //! Return function of IMU Unit Return Gyro Signal
 /*!
  Function get the angles of the 3-axis from the gyro in degree and return the 
  angle from x-axis back
*/
 uint16_t CIMUUnit::ReturnGyro(void){
      //Get the angles in degree 
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
      //Return the angle from x-axis in degree
      return (int16_t)euler.x();
 }

 
