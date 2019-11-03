#include <Stepper.h>

Stepper stepperL(2048, 8, 10, 9, 11);
Stepper stepperR(2048, 4, 6, 5, 7);

void setup() {
  stepperL.setSpeed(10);
  stepperR.setSpeed(10);
}

void loop() {
  stepperL.step(2048);
  stepperR.step(2048);
  delay(500);
  stepperL.step(-2048);
  stepperR.step(-2048);
  delay(500);
  stepperL.step(2048);
  stepperR.step(-2048);
  delay(500);
  stepperL.step(-2048);
  stepperR.step(2048);
  delay(500);
}
