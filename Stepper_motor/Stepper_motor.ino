#include <Stepper.h>  

Stepper StepperL(2048, 8, 10, 9, 11);
Stepper StepperR(2048, 4, 6, 5, 7);

void setup() {
  StepperL.setSpeed(10);  //設定馬達RPM
  StepperR.setSpeed(10);
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  // step one revolution  in one direction:
  Serial.println("OPEN");
  StepMotorOpen();
  delay(500);

  // step one revolution in the other direction:
  Serial.println("CLOSE");
  StepMotorClose();
  delay(500);
}

void StepMotorOpen(){
  int i;
  for(i=0;i<=1024;i++){
    StepperL.step(1);
    StepperR.step(-1);
  }
  
  return;
}

void StepMotorClose(){
  int i;
  for(i=0;i<=1024;i++){
    StepperL.step(-1);
    StepperR.step(1);
  }
  
  return;
}
