/*H**********************************************************************
* FILENAME :        IMU.cpp
*
* DESCRIPTION : Gyro and Acceleration sensor
*************************************************************************/

#include "HAL_IMU.h"
#include <Arduino.h>

#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

CIMUUnit::CIMUUnit(IMU_ID_e ID):
  m_ID(ID)
{
  //do nothing
}
CIMUUnit::~CIMUUnit(){

}

void CIMUUnit::Init(void){
	/* Initialise the sensor */
      if (!bno.begin())
      {
        /* There was a problem detecting the BNO055 ... check your connections */
        Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
        while (1);
      }

      delay(1000);

      /* Display the current temperature */
        int8_t temp = bno.getTemp();
        Serial.print("Current Temperature: ");
        Serial.print(temp);
        Serial.println(" C");
        Serial.println("");

        bno.setExtCrystalUse(true);

        Serial.println("Calibration status values: 0=uncalibrated, 3=fully calibrated");
}

 uint16_t CIMUUnit::ReturnGyro(void){
      imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

      return (int16_t)euler.x();
 }