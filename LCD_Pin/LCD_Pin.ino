#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //設定LCD位置0x27,設定LCD大小為16*2

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  lcd.backlight(); //開啟背光
  lcd.print("Hello, World!"); //顯示Hello, World!
}

void loop() {
}
