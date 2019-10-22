/*
控制伺服馬達轉動
90度-->0度-->90度-->179度，間隔1秒，重覆循環。
Edit by Jago 2015/12/16
*/

#include <Servo.h>

Servo myServo; // 建立一個伺服馬達物件

void setup() {
myServo.attach(9); // 伺服馬達物件連接到接腳9
}

void loop() {
myServo.write(180);
delay(1000);
myServo.write(-180);
delay(1000);
}


//澄線接Pin 9
//紅線接 電源
//棕線接 地線
