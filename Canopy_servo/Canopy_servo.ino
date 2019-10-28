/*DHT11 Pin 2 VCC 3.3V
  水滴感測器 Pin A0
  I2C PIN SDA A4 SCL A5
  LCD 與 GY30並聯
  伺服馬達Pin 9
*/

//I2C腳位的的標頭檔(A5=SCL,A4=SDA)
#include <Wire.h> 
//使用I2C控制器控制標準LCD的聲明                
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <BH1750.h>
#include <Servo.h>

#define DHTPIN 2             //DHT輸入PIN腳為2
#define DHTTYPE DHT11        //選擇HDT的規格
//#define DHTTYPE DHT22      //DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21      //DHT 21  (AM2301)

//水滴感測的範圍
const int sensorMin = 0;    //sensor minimum
const int sensorMax = 1024; //sensor maximum

int motor = 0;    //伸出=1,收回=0

//初始化DHT11
DHT dht(DHTPIN, DHTTYPE);
//設定 LCD I2C 位址
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3,POSITIVE);
BH1750 lightMeter;
//建立一個 servo 物件，最多可建立 12個 servo
Servo myservo;


/*設定*/
void setup(){
  Wire.begin();
  dht.begin();
  myservo.attach(9);    //將servo連結到pin9
  lightMeter.begin();
  myservo.write(0);
  delay(15);
  
  lcd.backlight();
  lcd.begin(16,4);     //初始化 LCD，一行16的字元，共4行，預設開啟背光
  lcd.setCursor(0,0);  // 設定游標位置在第一行行首
  lcd.print(F("Hello"));
  lcd.clear();
}

/*主程式*/
void loop(){
    lcd.setCursor(0,0);
    temp();
    lcd.clear();
    drip();
    lcd.clear();
    GY30();
    lcd.clear();

    delay(1000);
    if(temp() == 1 || drip() == 1 || GY30() == 1){  //伸出=1,收回=0
      myservo.write(0);
      delay(1000);
      lcd.clear();
      lcd.print("OPEN");
    }
    else{
      myservo.write(90);
      delay(1000);
      lcd.clear();
      lcd.print("CLOSE");
    }
  }

/*溫濕度感測器模組*/
float temp(){
  delay(2000);        //必須至少delay 2 秒
  lcd.clear();
  lcd.setCursor(0,0); //設定游標位置在第一行行首
  lcd.print("DHT11 Temperature");

  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
   
  
  if(isnan(t)||isnan(f)){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read from DHT sensor!"); //檢測錯誤
    return;
  }
  else{
    //設定游標位置在第二行行首
    lcd.setCursor(0,1);
    lcd.print("Temp:");
    lcd.print(t);
    lcd.print("C ");
    lcd.print(f);
    lcd.print("F");
  }
  if(t>=35){
    motor=1;
  }
  else{
    motor=0;
  }
  
  return motor;
}

/*水滴感測器模組*/
float drip(){
  delay(2000);  //delay 2秒
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Water drop sensor");
  
  int sensorReading = analogRead(A0);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3); //range的範圍
  
  lcd.setCursor(0,1);
  
  switch (range) {
  case 0: //Sensor全濕
    motor=1;
    lcd.print("Flood");
    break;
  case 1: //Sensor上有水珠
    motor=1;
    lcd.print("Rain Warning");
    break;
  case 2: //Sensor是乾的
    motor=0;
    lcd.print("Not Raining");
    break;
  }
  
  return motor;
}

/*光強度感測器模組*/
float GY30(){
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  float lux = lightMeter.readLightLevel();
  
  lcd.print("Light:");
  lcd.print(lux);
  lcd.print(" lx");

  if(lux>200){
    motor=1;
  }
  else{
    motor=0;
  }
  
  return motor;
}
