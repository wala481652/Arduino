#include <IRremote.h>

#define LED 13
#define OPEN 0x511DBB
#define CLOSE 0xEE886D7F

IRrecv irrecv(3); // Receive on pin 3
decode_results results;

const int in;

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  Serial.println("OPEN");
  irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn(); // 啟動紅外線接收器
  delay(1000);
}
void loop() {
  if (irrecv.decode(&results)){
    switch(results.value){
      case OPEN:
        Serial.println("LED OPEN");
        digitalWrite(LED,HIGH);
        Serial.println(results.value, HEX);
        break;
      case CLOSE:
        Serial.println("LED CLOSE");
        digitalWrite(LED,LOW);
        Serial.println(results.value, HEX);
        break;
    }
    irrecv.resume();
  }
  delay(100);
}
