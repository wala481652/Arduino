/*
  Example of BH1750 library usage.
  This example initialises the BH1750 object using the default high resolution
  continuous mode and then makes a light level reading every second.
  Connection:
    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND
  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).
*/
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <BH1750.h>

BH1750 lightMeter;
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE); 

void setup(){
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);

  lightMeter.begin();
  lcd.backlight();
  lcd.begin(16,4);  //初始化 LCD，一行16的字元，共4行，預設開啟背光
  lcd.setCursor(0,0);

  lcd.println(F("BH1750 Test begin"));
  lcd.clear();
}
void loop(){
  float lux = lightMeter.readLightLevel();
  lcd.print("Light: ");
  lcd.print(lux);
  lcd.println(" lx");
  delay(1000);
  lcd.clear();
}
