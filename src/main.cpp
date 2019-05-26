#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
void dataBase();
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

void menu()
{
  // prepare for menu 
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

BLYNK_WRITE(V3)
{
  totalStep = param.asInt();
}
BLYNK_WRITE(V4)
{
  str = param.asString();
}

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
  Blynk.begin(auth, ssid, pass);
  for (int i = 0; i < 3; i++)
  {
    dimmerStep = stepCalculate();
    dimmerStepDown = stepCalculateMin();
  }
  dataBase();
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  menu();
}
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

  cArray[4].name = "black";
  cArray[4].rgb[0] = 0;
  cArray[4].rgb[1] = 0;
  cArray[4].rgb[2] = 0;

  cArray[5].name = "yellow";
  cArray[5].rgb[0] = 255 * 4;
  cArray[5].rgb[1] = 255 * 4;
  cArray[5].rgb[2] = 0;

  cArray[6].name = "aqua";
  cArray[6].rgb[0] = 0;
  cArray[6].rgb[1] = 255 * 4;
  cArray[6].rgb[2] = 255 * 4;

  cArray[7].name = "magenta";
  cArray[7].rgb[0] = 255 * 4;
  cArray[7].rgb[1] = 0;
  cArray[7].rgb[2] = 255 * 4;

  cArray[8].name = "silver";
  cArray[8].rgb[0] = 192 * 4;
  cArray[8].rgb[1] = 192 * 4;
  cArray[8].rgb[2] = 192 * 4;

  cArray[9].name = "gray";
  cArray[9].rgb[0] = 128 * 4;
  cArray[9].rgb[1] = 128 * 4;
  cArray[9].rgb[2] = 128 * 4;

  cArray[10].name = "maroon";
  cArray[10].rgb[0] = 128 * 4;
  cArray[10].rgb[1] = 0;
  cArray[10].rgb[2] = 0;

  cArray[11].name = "olive";
  cArray[11].rgb[0] = 128 * 4;
  cArray[11].rgb[1] = 128 * 4;
  cArray[11].rgb[2] = 0;

  cArray[12].name = "lime";
  cArray[12].rgb[0] = 0;
  cArray[12].rgb[1] = 128 * 4;
  cArray[12].rgb[2] = 0;

  cArray[13].name = "purple";
  cArray[13].rgb[0] = 128 * 4;
  cArray[13].rgb[1] = 0;
  cArray[13].rgb[2] = 128 * 4;

  cArray[14].name = "teal";
  cArray[14].rgb[0] = 128 * 4;
  cArray[14].rgb[1] = 0;
  cArray[14].rgb[2] = 128 * 4;

  cArray[15].name = "navy";
  cArray[15].rgb[0] = 0;
  cArray[15].rgb[1] = 0;
  cArray[15].rgb[2] = 128 * 4;

  cArray[16].name = "dark red";
  cArray[16].rgb[0] = 139 * 4;
  cArray[16].rgb[1] = 0;
  cArray[16].rgb[2] = 0;

  cArray[17].name = "brown";
  cArray[17].rgb[0] = 165 * 4;
  cArray[17].rgb[1] = 42 * 4;
  cArray[17].rgb[2] = 42 * 4;

  cArray[18].name = "firebrick";
  cArray[18].rgb[0] = 178 * 4;
  cArray[18].rgb[1] = 34 * 4;
  cArray[18].rgb[2] = 34 * 4;

  cArray[19].name = "crimson";
  cArray[19].rgb[0] = 220 * 4;
  cArray[19].rgb[1] = 20 * 4;
  cArray[19].rgb[2] = 60 * 4;

  cArray[20].name = "tomato";
  cArray[20].rgb[0] = 255 * 4;
  cArray[20].rgb[1] = 99 * 4;
  cArray[20].rgb[2] = 71 * 4;

  cArray[21].name = "coral";
  cArray[21].rgb[0] = 255 * 4;
  cArray[21].rgb[1] = 127 * 4;
  cArray[21].rgb[2] = 80 * 4;

  cArray[22].name = "indian red";
  cArray[22].rgb[0] = 205 * 4;
  cArray[22].rgb[1] = 92 * 4;
  cArray[22].rgb[2] = 92 * 4;

  cArray[23].name = "light coral";
  cArray[23].rgb[0] = 240 * 4;
  cArray[23].rgb[1] = 128 * 4;
  cArray[23].rgb[2] = 128 * 4;

  cArray[24].name = "dark salmon";
  cArray[24].rgb[0] = 233 * 4;
  cArray[24].rgb[1] = 150 * 4;
  cArray[24].rgb[2] = 122 * 4;

  cArray[25].name = "salmon";
  cArray[25].rgb[0] = 205 * 4;
  cArray[25].rgb[1] = 92 * 4;
  cArray[25].rgb[2] = 92 * 4;

  cArray[26].name = "light salmon";
  cArray[26].rgb[0] = 205 * 4;
  cArray[26].rgb[1] = 92 * 4;
  cArray[26].rgb[2] = 92 * 4;

  cArray[27].name = "orange red";
  cArray[27].rgb[0] = 255 * 4;
  cArray[27].rgb[1] = 69 * 4;
  cArray[27].rgb[2] = 0 * 4;

  cArray[28].name = "dark orange";
  cArray[28].rgb[0] = 255 * 4;
  cArray[28].rgb[1] = 140 * 4;
  cArray[28].rgb[2] = 0 * 4;

  cArray[29].name = "orange";
  cArray[29].rgb[0] = 255 * 4;
  cArray[29].rgb[1] = 165 * 4;
  cArray[29].rgb[2] = 0 * 4;

  cArray[30].name = "gold";
  cArray[30].rgb[0] = 255 * 4;
  cArray[30].rgb[1] = 215 * 4;
  cArray[30].rgb[2] = 0 * 4;

  cArray[31].name = "dark golden rod";
  cArray[31].rgb[0] = 184 * 4;
  cArray[31].rgb[1] = 134 * 4;
  cArray[31].rgb[2] = 11 * 4;

  cArray[32].name = "golden rod";
  cArray[32].rgb[0] = 218 * 4;
  cArray[32].rgb[1] = 165 * 4;
  cArray[32].rgb[2] = 32 * 4;

  cArray[33].name = "pale golden rod";
  cArray[33].rgb[0] = 238 * 4;
  cArray[33].rgb[1] = 232 * 4;
  cArray[33].rgb[2] = 170 * 4;

  cArray[34].name = "dark khaki";
  cArray[34].rgb[0] = 189 * 4;
  cArray[34].rgb[1] = 183 * 4;
  cArray[34].rgb[2] = 107 * 4;

  cArray[35].name = "khaki";
  cArray[35].rgb[0] = 240 * 4;
  cArray[35].rgb[1] = 230 * 4;
  cArray[35].rgb[2] = 140 * 4;

  cArray[36].name = "olive";
  cArray[36].rgb[0] = 128 * 4;
  cArray[36].rgb[1] = 128 * 4;
  cArray[36].rgb[2] = 0 * 4;

  cArray[37].name = "yellow green";
  cArray[37].rgb[0] = 154 * 4;
  cArray[37].rgb[1] = 205 * 4;
  cArray[37].rgb[2] = 50 * 4;

  cArray[38].name = "dark olive green";
  cArray[38].rgb[0] = 85 * 4;
  cArray[38].rgb[1] = 107 * 4;
  cArray[38].rgb[2] = 47 * 4;

  cArray[39].name = "olive drab";
  cArray[39].rgb[0] = 107 * 4;
  cArray[39].rgb[1] = 142 * 4;
  cArray[39].rgb[2] = 35 * 4;

  cArray[40].name = "lawn green";
  cArray[40].rgb[0] = 124 * 4;
  cArray[40].rgb[1] = 252 * 4;
  cArray[40].rgb[2] = 0 * 4;

  cArray[41].name = "green yellow";
  cArray[41].rgb[0] = 173 * 4;
  cArray[41].rgb[1] = 255 * 4;
  cArray[41].rgb[2] = 47 * 4;

  cArray[42].name = "dark green";
  cArray[42].rgb[0] = 0 * 4;
  cArray[42].rgb[1] = 100 * 4;
  cArray[42].rgb[2] = 0 * 4;

  cArray[43].name = "forest green";
  cArray[43].rgb[0] = 34 * 4;
  cArray[43].rgb[1] = 139 * 4;
  cArray[43].rgb[2] = 34 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[45].name = "light green";
  cArray[45].rgb[0] = 144 * 4;
  cArray[45].rgb[1] = 238 * 4;
  cArray[45].rgb[2] = 144 * 4;

  cArray[46].name = "dark sea green";
  cArray[46].rgb[0] = 143 * 4;
  cArray[46].rgb[1] = 188 * 4;
  cArray[46].rgb[2] = 143 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50 * 4;
  cArray[44].rgb[1] = 205 * 4;
  cArray[44].rgb[2] = 50 * 4;
}
