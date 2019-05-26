#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

char auth[] = "b55b6f1b3a23440e97023a8584a9bde3";
char ssid[] = "DrZin";
char pass[] = "0985626152a";

#define VCC D2
#define RED D8
#define GREEN D6
#define BLUE D5
#define C D0
#define B D1
#define A D3
int red = 120;
int green = 120;
int blue = 0;
int vcc = 1024;
boolean lightStatus = false;
int lighStep = 0;
int i = 0;
int dimmerStep = 0;
int dimmerStepDown = 0;
int totalStep = 5;
boolean dimmerStart = false;
WidgetLCD lcd(V3);
boolean upStep = false;
boolean downStep = false;
boolean doUp = true;
boolean doDown = true;
String str = " ";
//*****************************//

int sensorValue(uint8_t c, uint8_t b, uint8_t a)
{
  digitalWrite(C, c);
  digitalWrite(B, b);
  digitalWrite(A, a);
  return analogRead(A0);
}

int avgIR()
{
  return (sensorValue(0, 1, 1) + sensorValue(0, 0, 1)) / 2;
}
void Menu()
{
}

int IRStep()
{
  if (avgIR() <= 1024 && avgIR() > 500 && doDown == true)
  {
    lcd.print(7, 1, "DownStep");
  }
  else if (avgIR() < 400 && avgIR() > 200 && doUp == true)
  {

    doDown = false;
    delay(100);
    if (avgIR() < 1024 && avgIR() > 500)
    {
      return 1;
    }
  }
}
int stepCalculate()
{
  return (1024 - max(max(red, green), blue)) / totalStep;
}

int stepCalculateMin()
{
  return (min(red, green) == 0 ? (min(red, blue) == 0 ? (min(green, blue) == 0 ? 0 : min(green, blue)) : min(red, blue)) : min(red, green)) / totalStep;
}

void print()
{
  Serial.print("R : ");
  Serial.print(red);
  Serial.print(" G : ");
  Serial.print(green);
  Serial.print(" B : ");
  Serial.print(blue);
  Serial.print(" Step : ");
  Serial.print(lighStep);
  Serial.print(" DimmerStepUP : ");
  Serial.print(dimmerStep);
  Serial.print(" DimmerStepDOWN : ");
  Serial.print(dimmerStepDown);
  Serial.println(" ");
}

void RGB()
{
  if (red == 1024 && green == 1024 && blue == 1024)
  {
    red = green = blue = 255;
  }
  analogWrite(VCC, vcc);
  analogWrite(RED, red);
  analogWrite(GREEN, green);
  analogWrite(BLUE, blue);
}

void openLight()
{
  vcc = lightStatus ? 1024 : 0;
}

void dimmer()
{
  if (lighStep > 0)
  {
    lighStep = 0;
    for (int i = 0; i < dimmerStep; i++)
    {
      if (red < 1024 && green < 1024 && blue < 1024)
      {
        red = red > 0 ? (red < 1024 ? red + 1 : 1024) : 0;
        green = green > 0 ? (green < 1024 ? green + 1 : 1024) : 0;
        blue = blue > 0 ? (blue < 1024 ? blue + 1 : 1024) : 0;
        RGB();
        delay(10);
      }
      dimmerStepDown = stepCalculateMin();
    }
  }
  else if (lighStep < 0)
  {
    lighStep = 0;
    for (int i = 0; i < dimmerStepDown; i++)
    {
      red = red > 0 ? (red < 1024 ? red - 1 : 1024) : 0;
      green = green > 0 ? (green < 1024 ? green - 1 : 1024) : 0;
      blue = blue > 0 ? (blue < 1024 ? blue - 1 : 1024) : 0;
      RGB();
      delay(10);
    }
    dimmerStep = stepCalculate();
  }
  else
  {
    RGB();
    print();
  }
}
struct color
{
  String name;
  int rgb[3];
};
color cArray[100];

void dataBase()
{
  cArray[0].name = "red";
  cArray[0].rgb[0] = 1024;
  cArray[0].rgb[1] = 0;
  cArray[0].rgb[2] = 0;

  cArray[1].name = "green";
  cArray[1].rgb[0] = 0;
  cArray[1].rgb[1] = 1024;
  cArray[1].rgb[2] = 0;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[3].name = "white";
  cArray[3].rgb[0] = 255;
  cArray[3].rgb[1] = 255;
  cArray[3].rgb[2] = 255;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;

  cArray[2].name = "blue";
  cArray[2].rgb[0] = 0;
  cArray[2].rgb[1] = 0;
  cArray[2].rgb[2] = 1024;
}

void search()
{

  str.toLowerCase();
  while (i < 100)
  {
    if (str == cArray[i].name)
    {
      red = cArray[i].rgb[0];
      green = cArray[i].rgb[1];
      blue = cArray[i].rgb[2];
      break;
    }
    i++;
    /* code */
  }
  i = 0;
}
//************************** Blynk Setting ****************************//

BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  lightStatus = pinValue == 1 ? true : false;
}
BLYNK_WRITE(V1)
{
  red = param[0].asInt();
  green = param[1].asInt();
  blue = param[2].asInt();
  dimmerStep = stepCalculate();
  dimmerStepDown = stepCalculateMin();
}
BLYNK_WRITE(V2)
{
  lighStep = param.asInt();
}

BLYNK_WRITE(V4)
{
  str = param.asString();
}
WidgetTerminal terminal(V3);
//************************** Blynk Setting ****************************//
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(VCC, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(A, OUTPUT);
  pinMode(D7, OUTPUT);
  // Blynk.begin(auth, ssid, pass);
  /* for (int i = 0; i < 3; i++)
  {
    dimmerStep = stepCalculate();
    dimmerStepDown = stepCalculateMin();
  }*/
  // dataBase();
}

void loop()
{
  // put your main code here, to run repeatedly:
  /*
  Blynk.run();
  openLight();
  lighStep = IRStep();
  dimmer();

 

  search();
 /* Serial.print(red);
  Serial.print(" ");
  Serial.print(green);
  Serial.print(" ");
  Serial.print(blue);
  Serial.print(str);
  Serial.print(" ");
  Serial.println(" ");
  delay(10);*/
 Serial.print(sensorValue(1,0,1));
 Serial.print(" ");
 Serial.print(sensorValue(1,0,0));
 Serial.println(" ");
}