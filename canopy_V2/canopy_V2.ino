#include<IRremote.h>
#include <Stepper.h>

#define OPEN 0xEE886D7F
#define CLOSE 0xE318261B
#define automode 0x511DBB

IRrecv irrecv(3); // Receive on pin 3
decode_results results;
//2個步進馬達的步數及PIN腳
Stepper StepperL(2048, 8, 10, 9, 11);
Stepper StepperR(2048, 4, 6, 5, 7);

int autoset = 0;    //預設自動模式啟動為1 關閉為0
int motor = 0;      //預設馬達伸出為1 縮回為0
int i = 0;

void setup() {
  StepperL.setSpeed(10);  //設定馬達RPM
  StepperR.setSpeed(10);
  irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn(); // 啟動紅外線接收器
  delay(1000);
}

void loop() {
  switch(irrecv.decode(&results)){
    case automode:  //按鍵(CH)
        irrecv.resume();
        break;
      case OPEN:  //按鍵(CH+)
        StepMotorOpen();
        irrecv.resume();
        break;
      case CLOSE:  //按鍵(CH-)
        StepMotorClose();
        irrecv.resume();
        break; 
  }
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
  int i;
  for (i = 1024; i >= 0; i--) {
    StepperL.step(-1);
    StepperR.step(1);
  }
  return;
}
