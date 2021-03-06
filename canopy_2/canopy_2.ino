/* DHT11 Pin 2 VCC 3.3V
   水滴感測器 Pin A0
   I2C PIN SDA A4 SCL A5
   LCD 與 GY30並聯
   紅外線感測器PIN 3 */

//I2C腳位的的標頭檔(A5=SCL,A4=SDA)
#include <Wire.h>
//使用I2C控制器控制標準LCD的聲明
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <BH1750.h>
#include <IRremote.h>
#include <Stepper.h>

#define DHTPIN 2      //DHT輸入PIN腳為2
#define DHTTYPE DHT11 //選擇HDT的規格
//#define DHTTYPE DHT22      //DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21      //DHT 21  (AM2301)
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
IRrecv irrecv(3); // Receive on pin 3
decode_results results;
//2個步進馬達的步數及PIN腳
Stepper StepperL(2048, 8, 10, 9, 11);
Stepper StepperR(2048, 4, 6, 5, 7);

//水滴感測的範圍
const int sensorMin = 0;    //sensor minimum
const int sensorMax = 1024; //sensor maximum
bool autoset = 0;           //預設自動模式啟動為1 關閉為0
bool T = 0, D = 0, L = 0;
int motor = 0, motorset = 0; //預設馬達啟動模式為1 關閉為0
bool X;

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
  lcd.print("Hello");
  StepMotorClose();
  lcd.clear();
  lcd.print("push the button");
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
        if (autoset == 1)
        {
          X = 1;
          lcd.clear();
          lcd.print("OPEN THE AUTOMODE");
        }
        else
        {
          X = 0;
          lcd.clear();
          lcd.print("CLOSE THE AUTOMODE");
          delay(2000);
          lcd.clear();
          lcd.print("push the button");
        }
        break;
      case OPEN: //按鍵(CH+)
        if (autoset != 1)
        {
          motorset = 1;
          lcd.clear();
          lcd.print("OPEN");
          StepMotorOpen();
          lcd.clear();
          lcd.print("push the button");
        }
        break;
      case CLOSE: //按鍵(CH-)
        if (autoset != 1)
        {
          motorset = 0;
          lcd.clear();
          lcd.print("CLOSE");
          StepMotorClose();
          lcd.clear();
          lcd.print("push the button");
        }
        break;
      case Temp: //按鍵(<<<)
        T = 1;
        if (autoset != 1)
        {
          lcd.clear();
          lcd.print("Temperature sensor");
          delay(1000);
          temp(T);
        }
        break;
      case Drip: //按鍵(>>>)
        D = 1;
        if (autoset != 1)
        {
          lcd.clear();
          lcd.print("Water drop sensor");
          delay(1000);
          drip(D);
        }
        break;
      case Light: //按鍵(>||)
        L = 1;
        if (autoset != 1)
        {
          lcd.clear();
          lcd.print("Light sensor");
          delay(1000);
          light(L);
        }
        break;
      default:
        break;
    }
    irrecv.resume();
  }
  if (autoset == 1) {
    autoMode();
  }
}
/*溫濕度感測器模組*/
int temp(int T)
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
int drip(int D)
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
int light(int L)
{
  float lux = lightMeter.readLightLevel();
  if (L == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print("Light:");
    lcd.print(lux);
    lcd.print(" lx");
  }

  if (lux > 200)
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
  for (i = 0; i <= 2048; i++)
  {
    StepperL.step(1);
    StepperR.step(-1);
  }
  return;
}

/*馬達反轉*/
void StepMotorClose()
{
  int i, stepL = -1, stepR = 1;
  for (i = 0; i <= 2048; i++)
  {
    StepperL.step(stepL);
    StepperR.step(stepR);
  }
  return;
}

void autoMode() {
  if (temp(1) == 1 || drip(1) == 1 || light(1) == 1) {
    if (X == 1) {
      lcd.clear();
      lcd.print("OPEN");
      StepMotorOpen();
      delay(1000);
      lcd.clear();
      X = 0;
    }
    else {
      delay(50);
    }
  }
  else {
    if (X == 0) {
      lcd.clear();
      lcd.print("CLOSE");
      StepMotorClose();
      delay(1000);
      lcd.clear();
      X = 1;
    }
    else {
      delay(50);
    }
  }
  return;
}
