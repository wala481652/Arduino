/* DHT11 Pin 2 VCC 3.3V
   水滴感測器 Pin A0
   I2C PIN SDA A4 SCL A5
   LCD 與 GY30並聯
   紅外線感測器PIN 3      */

//I2C腳位的的標頭檔(A5=SCL,A4=SDA)
#include <Wire.h>
//使用I2C控制器控制標準LCD的聲明
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
//DHT11的標頭檔
#include <DHT.h>
//光強度感測器的標頭檔
#include <BH1750.h>
//紅外線控制器的標頭檔
#include <IRremote.h>
//步進馬達的標頭檔
#include <Stepper.h>

#define DHTPIN 2             //DHT輸入PIN腳為2
#define DHTTYPE DHT11        //選擇HDT的規格
//#define DHTTYPE DHT22      //DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21      //DHT 21  (AM2301)
/*定義紅外線遙控器個按鍵之位址*/
#define OPEN 0x00FFE21D
#define CLOSE 0x00FFA25D
#define Automode 0x00FF629D
#define Temp 0x00FF22DD
#define Drip 0x00FF02FD
#define Light 0x00FFC23D

//初始化DHT11
DHT dht(DHTPIN, DHTTYPE);
//設定 LCD I2C 位址
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
BH1750 lightMeter;
//紅外線感測器
IRrecv irrecv(3); // Receive pin 3
decode_results results;
//2個步進馬達的步數及PIN腳
Stepper StepperL(2048, 8, 10, 9, 11);
Stepper StepperR(2048, 4, 6, 5, 7);

//水滴感測的範圍
const int sensorMin = 0;    //sensor minimum
const int sensorMax = 1024; //sensor maximum
//定義極限開關腳位
const int button1 = 0;
const int button2 = 1;
bool autoset = 0;           //預設自動模式啟動為1 關閉為0
bool T = 0, D = 0, L = 0;   //感測器是否顯示與LCD 0為顯示 1為關閉(T為溫度感測器 D為水滴感測器 L為光強度感測器)
bool motor = 0;             //預設馬達啟動模式為1 關閉為0
bool motorset;              //馬達目前狀態

/*設定*/
void setup()
{
  Wire.begin();
  dht.begin();
  lightMeter.begin();
  StepperL.setSpeed(10); //設定馬達RPM
  StepperR.setSpeed(10);
  irrecv.blink13(true); // 設為true的話，當收到訊號時，腳位13的LED便會閃爍
  irrecv.enableIRIn();  // 啟動紅外線接收器
  delay(1000);

  lcd.begin(16, 4);    //初始化 LCD，一行16的字元，共4行，預設開啟背光
  lcd.setCursor(0, 0); // 設定游標位置在第一行行首
  lcd.print(F("Hello"));
  StepMotorClose();
  lcd.clear();
  lcd.print(F("push the button"));
}

/*主程式*/
void loop()
{
  if (irrecv.decode(&results))
  {
    switch (results.value)
    {
      case Automode: //按鍵(CH)
        autoset = !autoset;
        if (autoset == 1) {
          lcd.clear();
          lcd.print("OPEN THE AUTOMODE");
        }
        else {
          lcd.clear();
          lcd.print("CLOSE THE AUTOMODE");
          delay(2000);
          lcd.clear();
          lcd.print("push the button");
        }
        break;
      case OPEN: //按鍵(CH+)
        T = 0;
        D = 0;
        L = 0;
        if (autoset != 1) {
          if (motorset == 0) {
            lcd.clear();
            lcd.print("OPEN");
            StepMotorOpen();
            lcd.clear();
            lcd.print("push the button");
          }
          else {
            lcd.clear();
            lcd.print("canopy is on");
            delay(2000);
            lcd.clear();
            lcd.print("push the button");
          }
        }
        break;
      case CLOSE: //按鍵(CH-)
        T = 0;
        D = 0;
        L = 0;
        if (autoset != 1) {
          if (motorset == 1) {
            lcd.clear();
            lcd.print("CLOSE");
            StepMotorClose();
            lcd.clear();
            lcd.print("push the button");
          }
          else {
            lcd.clear();
            lcd.print("canopy is off");
            delay(2000);
            lcd.clear();
            lcd.print("push the button");
          }
        }
        break;
      case Temp: //按鍵(<<<)
        T = 1;
        D = 0;
        L = 0;
        if (autoset != 1) {
          lcd.clear();
          lcd.print("Temperature sensor");
          delay(1000);
        }
        break;
      case Drip: //按鍵(>>>)
        D = 1;
        T = 0;
        L = 0;
        if (autoset != 1)
        {
          lcd.clear();
          lcd.print("Water drop sensor");
          delay(1000);
        }
        break;
      case Light: //按鍵(>||)
        L = 1;
        T = 0;
        D = 0;
        if (autoset != 1)
        {
          lcd.clear();
          lcd.print("Light sensor");
          delay(1000);
        }
        break;
      default:
        break;
    }
    irrecv.resume();
  }

  if (autoset == 1) {
    delay(50);
    autoMode();
  }
  else {
    if (T == 1) {
      delay(1500);
      temp(1);
    }
    if (D == 1) {
      delay(1500);
      drip(1);
    }
    if (L == 1) {
      delay(1500);
      light(1);
    }
  }
}
/*溫濕度感測器模組*/
bool temp(bool T)
{
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (T == 1)
  {
    if (isnan(t) || isnan(f))
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Failed to read from DHT sensor!"); //檢測錯誤
    }
    else
    {
      //設定游標位置在第二行行首
      lcd.setCursor(0, 1);
      lcd.print("Temp:");
      lcd.print(t);
      lcd.print("C ");
      lcd.print(f);
      lcd.print("F");
    }
  }
  if (t >= 30)
  {
    motor = 1;
  }
  else
  {
    motor = 0;
  }

  return motor;
}

/*水滴感測器模組*/
bool drip(bool D)
{
  int sensorReading = analogRead(A0);
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3); //range的範圍

  lcd.setCursor(0, 1);

  switch (range)
  {
    case 0: //Sensor全濕
      motor = 1;
      if (D == 1)
      {
        lcd.print("Flood");
      }
      break;
    case 1: //Sensor上有水珠
      motor = 1;
      if (D == 1)
      {
        lcd.print("Rain Warning");
      }
      break;
    case 2: //Sensor是乾的
      motor = 0;
      if (D == 1)
      {
        lcd.print("Not Raining");
      }
      break;
  }

  return motor;
}

/*光強度感測器模組*/
bool light(bool L)
{
  float lux = lightMeter.readLightLevel();
  if (L == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Light:");
    lcd.print(lux);
    lcd.print(" lx");
  }

  if (lux > 300)
  {
    motor = 1;
  }
  else
  {
    motor = 0;
  }

  return motor;
}

/*馬達正轉*/
void StepMotorOpen()
{
  int i;
  motorset = 1;
  for (i = 0; i <= 4500; i++)
  {
    StepperL.step(1);
    StepperR.step(-1);
  }
  return;
}

/*馬達反轉*/
void StepMotorClose()
{
  int i;
  motorset = 0;
  for (i = 0; i <= 4500; i++)
  {
    if (digitalRead(button2) == LOW && digitalRead(button1) == LOW) {
      break;
    }
    if (digitalRead(button1) == LOW) {
      delay(50);
      StepperR.step(100);
    }
    else if (digitalRead(button2) == LOW) {
      delay(50);
      StepperL.step(-100);
    }
    else {
      StepperL.step(-1);
      StepperR.step(1);
    }
  }
  return;
}

/*自動模式*/
void autoMode() {
  delay(50);
  if (temp(0) == 1 || drip(0) == 1 || light(0) == 1) {
    if (motorset == 0) {
      lcd.clear();
      lcd.print("OPEN");
      StepMotorOpen();
      delay(1000);
      lcd.clear();
      lcd.print("OPEN THE AUTOMODE");
    }
  }
  else {
    if (motorset == 1) {
      lcd.clear();
      lcd.print("CLOSE");
      StepMotorClose();
      delay(1000);
      lcd.clear();
      lcd.print("OPEN THE AUTOMODE");
    }
  }
  return;
}
