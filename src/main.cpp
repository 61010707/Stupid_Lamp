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
int red = 120;
int green = 120;
int blue = 0;
int vcc = 1024;
boolean lightStatus = false;
int lighStep = 0;
int i = 0;
int dimmerStep = 0;
int dimmerStepDown = 0;
int totalStep = 2;
boolean dimmerStart = false;
//*****************************//
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
      }
      dimmerStepDown = stepCalculateMin();
      print();
      RGB();
      delay(10);
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
      print();
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

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(VCC, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  Serial.println(WiFi.localIP());
  for (int i = 0; i < 3; i++)
  {
    dimmerStep = stepCalculate();
    dimmerStepDown = stepCalculateMin();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

  Blynk.run();
  openLight();
  dimmer();
}