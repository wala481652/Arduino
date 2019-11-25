#include <Stepper.h>

#define Button1 1

Stepper StepperL(2048, 8, 10, 9, 11);
Stepper StepperR(2048, 4, 6, 5, 7);

void setup() {
  pinMode(Button1, LOW);
  StepperL.setSpeed(10);  //設定馬達RPM
  StepperR.setSpeed(10);
  Serial.begin(9600);
}

void loop() {
  Serial.println("CLOSE");
  StepMotorClose();
  delay(500);
}

void StepMotorOpen() {
  int i;
  for (i = 0; i <= 1024; i++) {
    StepperL.step(1);
    StepperR.step(-1);
  }
  return;
}

void StepMotorClose() {
  int i, stepL = -1, stepR = 1;

  for (i = 0; i <= 1024; i++) {
    if (digitalRead(Button1) == LOW) {
      stepL = 0;
      stepR = 0;
    }
    else if (digitalRead(Button1)==HIGH) {
      stepL = -1;
      stepR = 1;
    }
    StepperL.step(stepL);
    StepperR.step(stepR);
  }
  return;
}
