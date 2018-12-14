#include "App_MotorCtrl.h"
#include "Arduino.h"
#include "HAL_PWM.h" //PWM header file
#include "HAL_IMU.h" //PWM header file
#include "App_Stateflow.h" // Model's header file
#include "App_Stateflowtypes.h" // Type definitions

#define PIN_ENC 3
#define PIN_ENABLE 52
#define PIN_DIRECTION_L 50
#define PIN_DIRECTION_R 48


int n = 20;
int16_T dist = 0;

//Encoder Definitions
unsigned long time;
unsigned long enc_last_time = 0;
unsigned long enc_t = 0;
uint16_t rpm = 0;

  
CMotorCtrl::CMotorCtrl(CIMUUnit& imu_o, CPWMUnit& pwmUnitLeft_o, CPWMUnit& pwmUnitRight_o, CPLSComms& plsCOmms_o):
rtObj(),
m_imu_o(imu_o),
m_pwmUnitLeft_o(pwmUnitLeft_o),
m_pwmUnitRight_o(pwmUnitRight_o),
m_plsCOmms_o(plsCOmms_o)
{

}
CMotorCtrl::~CMotorCtrl(){
  //do nothing
}

void CMotorCtrl::Init(void)
{
      //Setup PWM
      pinMode(PIN_ENABLE, OUTPUT);
      pinMode(PIN_DIRECTION_R, OUTPUT);
      pinMode(PIN_DIRECTION_L, OUTPUT);


      digitalWrite(PIN_ENABLE, LOW);
      digitalWrite(PIN_DIRECTION_R, HIGH);
      digitalWrite(PIN_DIRECTION_L, HIGH);

      pwmUnitLeft_o.setupPWM16();
      pwmUnitRight_o.setupPWM16();

      writeMOT(65535, 65535);

      // Initialize stateflow
      rtObj.initialize();
}

void CMotorCtrl::Run(void)
{
      //Stateflow
      if (rtmGetErrorStatus(rtObj.getRTM()) != (NULL)) {
        //  Called when error in stateflow
        Serial.print("StateFlow Error!");
        while (true) {}

      } else {

        //Set Gyro Signal
        rtObj.rtU.gyro_signal = m_imu_o.ReturnGyro();

        getUserInput();

        n++;

        // Call stateflow
        rt_OneStep();

        //PWM
        if (digitalRead(PIN_ENABLE)== HIGH) {
          digitalWrite(MOTR, LOW);
        }else{
          checkState();
        }

        printValues();
      }
}

void CMotorCtrl::Stop(void)
{
	//do nothing
}

void CMotorCtrl::checkState(void){
    switch(rtObj.rtDW.is_c3_Chart){
        case 1:
          Serial.println("State 1: Backward");
          digitalWrite(PIN_DIRECTION_L, HIGH);
          digitalWrite(PIN_DIRECTION_R, LOW);
          writeMOT(61500, 61500);
          break;
        case 2:
          Serial.println("State 2: Forward");
          digitalWrite(PIN_DIRECTION_L, LOW);
          digitalWrite(PIN_DIRECTION_R, HIGH);
          writeMOT(61500, 61500);
          break;
        case 3:
          Serial.println("State 3: IDLE");
          digitalWrite(PIN_DIRECTION_L, HIGH);
          digitalWrite(PIN_DIRECTION_R, HIGH);
          writeMOT(65535, 65535);
          break;
        case 4:
          Serial.println("State 4: Left Turn");
          digitalWrite(PIN_DIRECTION_L, LOW);
          digitalWrite(PIN_DIRECTION_R, HIGH);
          writeMOT(61500, 61500);
          break;
        case 5:
          Serial.println("State 5: Right Turn");
          digitalWrite(PIN_DIRECTION_L, HIGH);
          digitalWrite(PIN_DIRECTION_R, LOW);
          writeMOT(61500, 61500);
          break;
      }
  }


#ifdef ADAS_DEBUG
void CMotorCtrl::getUserInput(void){
    if (n == 20) {
      rtObj.rtU.turn = 0;
      rtObj.rtU.dist = 100;
    }
    if(n == 40){
      rtObj.rtU.turn = 0;
      rtObj.rtU.dist = 0;
    }
    if(n == 60){
      rtObj.rtU.turn = 0;
      rtObj.rtU.dist = -100;
    }
    if(n == 80){
      rtObj.rtU.turn = 0;
      rtObj.rtU.dist = 0;
    }
  }

void CMotorCtrl::printValues(void){
    Serial.println("____________________________________________________________________________ ");
    Serial.println("Inputs and local Variables: ");
    Serial.println("____________________________________________________________________________ ");
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
    Serial.println("____________________________________________________________________________ ");
    Serial.println("Outputs: ");
    Serial.println("____________________________________________________________________________ ");
    Serial.print("Mot_l: ");
    Serial.print(rtObj.rtY.mot_l);
    Serial.print(" || Dir_l: ");
    Serial.print(rtObj.rtY.dir_l);
    Serial.print(" || Mot_r: ");
    Serial.print(rtObj.rtY.mot_r);
    Serial.print(" || Dir_r: ");
    Serial.println(rtObj.rtY.dir_r);
}
#endif

// Function to perform a stateflow calculation
void CMotorCtrl::rt_OneStep(void)
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
