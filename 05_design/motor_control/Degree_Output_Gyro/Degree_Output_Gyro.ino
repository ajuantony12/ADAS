#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include "Chart.h" // Model's header file
#include "rtwtypes.h" // Type definitions

// Initialize stateflow object
static ChartModelClass rtObj;
int n = 20;
int16_T dist = 0;

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground

   History
   =======
   2015/MAR/03  - First release (KTOWN)
*/

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (100)

Adafruit_BNO055 bno = Adafruit_BNO055();

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  Serial.begin(9600);
  Serial.println("Orientation Sensor Test Started..."); Serial.println("");

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

  // Initialize stateflow
  rtObj.initialize();
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{

  if (rtmGetErrorStatus(rtObj.getRTM()) != (NULL)) {
    //  Called when error in stateflow
    Serial.print("StateFlow Error!");
    while (true) {}

  } else {
    // Called when no error in stateflow
    // Possible vector values can be:
    // - VECTOR_ACCELEROMETER - m/s^2
    // - VECTOR_MAGNETOMETER  - uT
    // - VECTOR_GYROSCOPE     - rad/s
    // - VECTOR_EULER         - degrees
    // - VECTOR_LINEARACCEL   - m/s^2
    // - VECTOR_GRAVITY       - m/s^2

    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

    //Define Inputs
    rtObj.rtU.gyro_signal = (int16_t)euler.x();

    if (n == 20) {
      Serial.println("Turn Signal: "); //Prompt User for Input
      bool schleife = true;
      while (schleife) { // Wait for User to Input Data
        rtObj.rtU.turn = Serial.parseInt(); //Read the data the user has input
        if (Serial.read() == '\n') {
          schleife = false;
        }
      }

      Serial.print("Turn signal set to: ");
      Serial.println(rtObj.rtU.turn);
      schleife = true;
      Serial.println("Distance: "); //Prompt User for Input
      while (schleife) { // Wait for User to Input Data
        rtObj.rtU.dist = Serial.parseInt();
        if (Serial.read() == '\n') {
          schleife = false;
        }
      }

      n = 0;
    }

    n++;

    // Call stateflow
    rt_OneStep();

    /* Display the floating point data */
    Serial.println("Inputs and local Variables: ");
    Serial.println("_______________________ ");
    Serial.print("gyro_signal: ");
    Serial.print(rtObj.rtU.gyro_signal);
    Serial.print(" || Curr_Angle: ");
    Serial.print(rtObj.rtDW.curr_angle);
    Serial.print(" || Turn: ");
    Serial.print(rtObj.rtU.turn);
    Serial.print(" || op: ");
    Serial.print(rtObj.rtDW.op);
    Serial.print(" || distance: ");
    Serial.println(rtObj.rtU.dist);
    Serial.println("_______________________ ");
    Serial.println("Outputs: ");
    Serial.println("_______________________ ");
    Serial.print("Mot_l: ");
    Serial.print(rtObj.rtY.mot_l);
    Serial.print(" || Dir_l: ");
    Serial.print(rtObj.rtY.dir_l);
    Serial.print(" || Mot_r: ");
    Serial.print(rtObj.rtY.mot_r);
    Serial.print(" || Dir_r: ");
    Serial.print(rtObj.rtY.dir_r);
    Serial.print(" || State: ");
    Serial.print(rtObj.rtDW.is_c3_Chart);
    Serial.println("_______________________ ");


    /*
      // Quaternion data
      imu::Quaternion quat = bno.getQuat();
      Serial.print("qW: ");
      Serial.print(quat.w(), 4);
      Serial.print(" qX: ");
      Serial.print(quat.y(), 4);
      Serial.print(" qY: ");
      Serial.print(quat.x(), 4);
      Serial.print(" qZ: ");
      Serial.print(quat.z(), 4);
      Serial.print("\t\t");
    */

    /* Display calibration status for each sensor.
      uint8_t system, gyro, accel, mag = 0;
      bno.getCalibration(&system, &gyro, &accel, &mag);
      Serial.print(" || CALIBRATION: Sys=");
      Serial.print(system, DEC);
      Serial.print(" Gyro=");
      Serial.print(gyro, DEC);
      Serial.print(" Accel=");
      Serial.print(accel, DEC);
      Serial.print(" Mag=");
      Serial.println(mag, DEC);*/

    delay(BNO055_SAMPLERATE_DELAY_MS);
  }


}

int getTurn() {

}

// Function to perform a stateflow calculation
void rt_OneStep(void)
{
  static boolean_T OverrunFlag = false;

  // Disable interrupts here

  // Check for overrun
  if (OverrunFlag) {
    rtmSetErrorStatus(rtObj.getRTM(), "Overrun");
    return;
  }

  OverrunFlag = true;

  // Save FPU context here (if necessary)
  // Re-enable timer or interrupt here
  // Set model inputs here

  // Step the model
  rtObj.step();

  // Get model outputs here

  // Indicate task complete
  OverrunFlag = false;

  // Disable interrupts here
  // Restore FPU context here (if necessary)
  // Enable interrupts here
}
