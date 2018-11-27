#include "Chart.h" // Model's header file
#include "rtwtypes.h" // Type definitions

#define BTN1 8
#define BTN2 9
#define LED  11

// Initialize stateflow object
static ChartModelClass rtObj;

void setup() {
  Serial.begin(115200);
  Serial.println("Started...");

  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Initialize stateflow
  rtObj.initialize();


}

void loop() {

  if rtmGetErrorStatus(rtObj.getRTM()) != (NULL) {
    //  Called when error in stateflow
    Serial.print("StateFlow Error!");
    while (true) {}

  } else {
    // Called when no error in stateflow

    // Write inputs to stateflow
    rtObj.rtU.I1 = digitalRead(BTN1);
    rtObj.rtU.I2 = digitalRead(BTN2);

    // Call stateflow
    rt_OneStep();

    // Write output value to output pins
    digitalWrite(LED, rtObj.rtY.Out);
  }




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

