#include <LiquidCrystal_I2C.h> 
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,3);  //設定LCD位置0x27,設定LCD大小為16*2

void setup() {
  // put your setup code here, to run once:
  lcd.init(); //初始化LCD 
  lcd.backlight(); //開啟背光
  lcd.setCursor(0,0);
  lcd.print("Hello,World!"); //顯示Hello, World!
  lcd.setCursor(0,1);
  lcd.print("123456");
  lcd.setCursor(0,2);
  lcd.print("123456");
  lcd.setCursor(0,3);
  lcd.print("123456"); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
