#include <IRremote.h>

#define LED 13
#define OPEN 0x00FF30CF
#define CLOSE 0x00FF18E7

IRrecv irrecv(4); // Receive on pin 4
decode_results results;

const int in;

void setup() {
  Serial.begin(115200);
  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);
  Serial.println("OPEN");
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
