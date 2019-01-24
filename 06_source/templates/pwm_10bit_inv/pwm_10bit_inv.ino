#include "HAL_PWM.h"

CPWMUnit    pwmUnitLeft_o(CPWMUnit::PWM1);
CPWMUnit    pwmUnitRight_o(CPWMUnit::PWM2);

void setup() {
  pwmUnitLeft_o.setupPWM10();
  pwmUnitRight_o.setupPWM10();

  pwmUnitLeft_o.writeMOT(30);
  pwmUnitRight_o.writeMOT(30);

}

void loop() {

}
