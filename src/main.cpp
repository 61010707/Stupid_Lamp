#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
void dataBase();
void openLight();
char auth[] = "b55b6f1b3a23440e97023a8584a9bde3";
char ssid[] = "DrZin_hotspot";
char pass[] = "0985626152a";

#define VCC D2
#define RED D8
#define GREEN D6
#define BLUE D5
#define C D0
#define B D1
#define A D3
int red = 255;
int green = 255;
int blue = 255;
int vcc = 1024;
boolean lightStatus = false;
int lighStep = 0;
int i = 0;
int dimmerStep = 0;
int dimmerStepDown = 0;
int totalStep = 2;
boolean dimmerStart = false;
boolean upStep = false;
boolean downStep = false;
boolean doUp = true;
boolean doDown = true;
String str = " ";
boolean downLight = true;
boolean uplight = false;
boolean mode = false;
unsigned long timer;
unsigned long startTime;
boolean isTouch = false;
int startPoint;
boolean inLoop = false;
boolean isDim = false;
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
  openLight();
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
  }
  lighStep = 0;
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
      str = " ";
    }
    i++;
    /* code */
  }
  i = 0;
}

void lightMode()
{
  if (downLight)
  {
    red = red > 0 ? red - 1 : red;
    green = red == 0 && green > 0 ? green - 1 : green;
    blue = green == 0 && blue > 0 ? blue - 1 : blue;
    if (red == 0 && green == 0 && blue == 0)
    {
      downLight = false;
      uplight = true;
    }
  }
  if (uplight)
  {
    red = red < 1024 ? red + 1 : red;
    green = red == 1024 && green < 1024 ? green + 1 : green;
    blue = green == 1024 && blue < 1024 ? blue + 1 : blue;
    if (red == 1024 && green == 1024 && blue == 1024)
    {
      uplight = false;
      downLight = true;
      red = green = blue = 1024;
    }
  }
  RGB();
  print();
  delay(30);
}

void menu()
{
  RGB();
  if (mode)
  {
    lightMode();
  }
  else
  {
    RGB();
    dimmer();
    if (str != " ")
    {
      search();
      str = " ";
    }

    if (sensorValue(1, 0, 1) == 1024 && isTouch == false)
    {
      startTime = millis();
      isTouch = true;
      Serial.print("in touch");
    }

    if (isTouch == true && sensorValue(1, 0, 1) == 1024 && millis() - startTime > 3000)
    {
      Serial.print("in menu");
      if (inLoop == false)
      {
        for (int i = 0; i < 4; i++)
        {
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
        }
        inLoop = true;
      }

      delay(1000);
      if (sensorValue(1, 0, 1) < 400)
      {
        Serial.print("dimmer up");
        delay(1200);
        if (sensorValue(1, 0, 1) > 600)
        {
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          lighStep = 1;
          dimmer();
          lighStep = 0;
          isTouch = false;
          inLoop = false;
        }
        else
        {
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          lighStep = 1;
          dimmer();
          lighStep = 0;
          isTouch = false;
          inLoop = false;
        }
      }
      if (sensorValue(1, 0, 1) > 650)
      {
        Serial.print("dimmer up");
        delay(500);
        if (sensorValue(1, 0, 1) < 500)
        {
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          lighStep = -1;
          dimmer();
          lighStep = 0;
          isTouch = false;
          inLoop = false;
        }
        else
        {
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          analogWrite(VCC, 0);
          delay(1000);
          analogWrite(VCC, 1024);
          delay(1000);
          lighStep = -1;
          dimmer();
          lighStep = 0;
          isTouch = false;
          inLoop = false;
        }
      }
    }
  }

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
BLYNK_WRITE(V5)
{
  String str = param.asString();
  str.toLowerCase();
  if (str.equals("on"))
  {
    mode = true;
  }
  else
  {
    mode = false;
  }
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
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  Serial.println(WiFi.localIP());
}

void loop()
{
  Blynk.run();
  if (mode)
  {
    lightMode();
  }
  else
  {
    RGB();
    dimmer();
    if (str != " ")
    {
      search();
      str = " ";
    }
    int check = sensorValue(1, 0, 1);
    Serial.println(check);
    delay(3);
    if (check >= 1024 && isTouch == false)
    {
      startTime = millis();
      isTouch = true;
      Serial.print("in touch");
    }
  }

  int checkSensor = sensorValue(1, 0, 1);
  delay(3);
  if (isTouch == true && checkSensor == 1024 && millis() - startTime > 3000)
  {
    Serial.print("in menu");
    if (inLoop == false)
    {
      for (int i = 0; i < 4; i++)
      {
        analogWrite(RED, 255);
        analogWrite(GREEN, 255);
        analogWrite(BLUE, 255);
        analogWrite(VCC, 0);
        delay(1000);
        analogWrite(VCC, 1024);
        delay(1000);
      }
      inLoop = true;
    }
    delay(1000);
    int readValue = sensorValue(1, 0, 1);
    delay(3);
    if (readValue < 500)
    {
      Serial.print("dimmer up");

      analogWrite(VCC, 0);
      delay(1000);
      analogWrite(VCC, 1024);
      delay(1000);
      lighStep = 1;
      dimmer();
      lighStep = 0;
      isTouch = false;
      inLoop = false;
    }
    else if (readValue > 600)
    {
      Serial.print("dimmer down");

      analogWrite(VCC, 0);
      delay(1000);
      analogWrite(VCC, 1024);
      delay(1000);
      lighStep = -1;
      dimmer();
      lighStep = 0;
      isTouch = false;
      inLoop = false;
    }
    else
    {
      analogWrite(VCC, 0);
      delay(1000);
      analogWrite(VCC, 1024);
      delay(1000);
      lighStep = 1;
      dimmer();
      lighStep = 0;
      isTouch = false;
      inLoop = false;
    }
  }
  delay(3);
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
  cArray[5].rgb[0] = 255;
  cArray[5].rgb[1] = 255;
  cArray[5].rgb[2] = 0;

  cArray[6].name = "aqua";
  cArray[6].rgb[0] = 0;
  cArray[6].rgb[1] = 255;
  cArray[6].rgb[2] = 255;

  cArray[7].name = "magenta";
  cArray[7].rgb[0] = 255;
  cArray[7].rgb[1] = 0;
  cArray[7].rgb[2] = 255;

  cArray[8].name = "silver";
  cArray[8].rgb[0] = 192;
  cArray[8].rgb[1] = 192;
  cArray[8].rgb[2] = 192;

  cArray[9].name = "gray";
  cArray[9].rgb[0] = 128;
  cArray[9].rgb[1] = 128;
  cArray[9].rgb[2] = 128;

  cArray[10].name = "maroon";
  cArray[10].rgb[0] = 128;
  cArray[10].rgb[1] = 0;
  cArray[10].rgb[2] = 0;

  cArray[11].name = "olive";
  cArray[11].rgb[0] = 128;
  cArray[11].rgb[1] = 128;
  cArray[11].rgb[2] = 0;

  cArray[12].name = "lime";
  cArray[12].rgb[0] = 0;
  cArray[12].rgb[1] = 128;
  cArray[12].rgb[2] = 0;

  cArray[13].name = "purple";
  cArray[13].rgb[0] = 128;
  cArray[13].rgb[1] = 0;
  cArray[13].rgb[2] = 128;

  cArray[14].name = "teal";
  cArray[14].rgb[0] = 128;
  cArray[14].rgb[1] = 0;
  cArray[14].rgb[2] = 128;

  cArray[15].name = "navy";
  cArray[15].rgb[0] = 0;
  cArray[15].rgb[1] = 0;
  cArray[15].rgb[2] = 128;

  cArray[16].name = "dark red";
  cArray[16].rgb[0] = 139;
  cArray[16].rgb[1] = 0;
  cArray[16].rgb[2] = 0;

  cArray[17].name = "brown";
  cArray[17].rgb[0] = 165;
  cArray[17].rgb[1] = 42;
  cArray[17].rgb[2] = 42;

  cArray[18].name = "firebrick";
  cArray[18].rgb[0] = 178;
  cArray[18].rgb[1] = 34;
  cArray[18].rgb[2] = 34;

  cArray[19].name = "crimson";
  cArray[19].rgb[0] = 220;
  cArray[19].rgb[1] = 20;
  cArray[19].rgb[2] = 60;

  cArray[20].name = "tomato";
  cArray[20].rgb[0] = 255;
  cArray[20].rgb[1] = 99;
  cArray[20].rgb[2] = 71;

  cArray[21].name = "coral";
  cArray[21].rgb[0] = 255;
  cArray[21].rgb[1] = 127;
  cArray[21].rgb[2] = 80;

  cArray[22].name = "indian red";
  cArray[22].rgb[0] = 205;
  cArray[22].rgb[1] = 92;
  cArray[22].rgb[2] = 92;

  cArray[23].name = "light coral";
  cArray[23].rgb[0] = 240;
  cArray[23].rgb[1] = 128;
  cArray[23].rgb[2] = 128;

  cArray[24].name = "dark salmon";
  cArray[24].rgb[0] = 233;
  cArray[24].rgb[1] = 150;
  cArray[24].rgb[2] = 122;

  cArray[25].name = "salmon";
  cArray[25].rgb[0] = 205;
  cArray[25].rgb[1] = 92;
  cArray[25].rgb[2] = 92;

  cArray[26].name = "light salmon";
  cArray[26].rgb[0] = 205;
  cArray[26].rgb[1] = 92;
  cArray[26].rgb[2] = 92;

  cArray[27].name = "orange red";
  cArray[27].rgb[0] = 255;
  cArray[27].rgb[1] = 69;
  cArray[27].rgb[2] = 0;

  cArray[28].name = "dark orange";
  cArray[28].rgb[0] = 255;
  cArray[28].rgb[1] = 140;
  cArray[28].rgb[2] = 0;

  cArray[29].name = "orange";
  cArray[29].rgb[0] = 255;
  cArray[29].rgb[1] = 165;
  cArray[29].rgb[2] = 0;

  cArray[30].name = "gold";
  cArray[30].rgb[0] = 255;
  cArray[30].rgb[1] = 215;
  cArray[30].rgb[2] = 0;

  cArray[31].name = "dark golden rod";
  cArray[31].rgb[0] = 184;
  cArray[31].rgb[1] = 134;
  cArray[31].rgb[2] = 11;

  cArray[32].name = "golden rod";
  cArray[32].rgb[0] = 218;
  cArray[32].rgb[1] = 165;
  cArray[32].rgb[2] = 32;

  cArray[33].name = "pale golden rod";
  cArray[33].rgb[0] = 238;
  cArray[33].rgb[1] = 232;
  cArray[33].rgb[2] = 170;

  cArray[34].name = "dark khaki";
  cArray[34].rgb[0] = 189;
  cArray[34].rgb[1] = 183;
  cArray[34].rgb[2] = 107;

  cArray[35].name = "khaki";
  cArray[35].rgb[0] = 240;
  cArray[35].rgb[1] = 230;
  cArray[35].rgb[2] = 140;

  cArray[36].name = "olive";
  cArray[36].rgb[0] = 128;
  cArray[36].rgb[1] = 128;
  cArray[36].rgb[2] = 0;

  cArray[37].name = "yellow green";
  cArray[37].rgb[0] = 154;
  cArray[37].rgb[1] = 205;
  cArray[37].rgb[2] = 50;

  cArray[38].name = "dark olive green";
  cArray[38].rgb[0] = 85;
  cArray[38].rgb[1] = 107;
  cArray[38].rgb[2] = 47;

  cArray[39].name = "olive drab";
  cArray[39].rgb[0] = 107;
  cArray[39].rgb[1] = 142;
  cArray[39].rgb[2] = 35;

  cArray[40].name = "lawn green";
  cArray[40].rgb[0] = 124;
  cArray[40].rgb[1] = 252;
  cArray[40].rgb[2] = 0;

  cArray[41].name = "green yellow";
  cArray[41].rgb[0] = 173;
  cArray[41].rgb[1] = 255;
  cArray[41].rgb[2] = 47;

  cArray[42].name = "dark green";
  cArray[42].rgb[0] = 0;
  cArray[42].rgb[1] = 100;
  cArray[42].rgb[2] = 0;

  cArray[43].name = "forest green";
  cArray[43].rgb[0] = 34;
  cArray[43].rgb[1] = 139;
  cArray[43].rgb[2] = 34;

  cArray[44].name = "lime green";
  cArray[44].rgb[0] = 50;
  cArray[44].rgb[1] = 205;
  cArray[44].rgb[2] = 50;

  cArray[45].name = "light green";
  cArray[45].rgb[0] = 144;
  cArray[45].rgb[1] = 238;
  cArray[45].rgb[2] = 144;

  cArray[46].name = "dark sea green";
  cArray[46].rgb[0] = 143;
  cArray[46].rgb[1] = 188;
  cArray[46].rgb[2] = 143;

  cArray[47].name = "spring green";
  cArray[47].rgb[0] = 0;
  cArray[47].rgb[1] = 255;
  cArray[47].rgb[2] = 127;

  cArray[48].name = "sea green";
  cArray[48].rgb[0] = 46;
  cArray[48].rgb[1] = 139;
  cArray[48].rgb[2] = 87;

  cArray[49].name = "light sea green";
  cArray[49].rgb[0] = 32;
  cArray[49].rgb[1] = 178;
  cArray[49].rgb[2] = 170;

  cArray[50].name = "lime green";
  cArray[50].rgb[0] = 50;
  cArray[50].rgb[1] = 205;
  cArray[50].rgb[2] = 50;

  cArray[51].name = "dark cyan";
  cArray[51].rgb[0] = 0;
  cArray[51].rgb[1] = 139;
  cArray[51].rgb[2] = 139;

  cArray[52].name = "aqua marine";
  cArray[52].rgb[0] = 50;
  cArray[52].rgb[1] = 205;
  cArray[52].rgb[2] = 50;

  cArray[53].name = "cadet blue";
  cArray[53].rgb[0] = 95;
  cArray[53].rgb[1] = 158;
  cArray[53].rgb[2] = 160;

  cArray[54].name = "steel blue";
  cArray[54].rgb[0] = 70;
  cArray[54].rgb[1] = 130;
  cArray[54].rgb[2] = 180;

  cArray[55].name = "deep sky blue";
  cArray[55].rgb[0] = 50;
  cArray[55].rgb[1] = 205;
  cArray[55].rgb[2] = 50;

  cArray[56].name = "light blue";
  cArray[56].rgb[0] = 173;
  cArray[56].rgb[1] = 216;
  cArray[56].rgb[2] = 230;

  cArray[57].name = "sky blue";
  cArray[57].rgb[0] = 135;
  cArray[57].rgb[1] = 206;
  cArray[57].rgb[2] = 235;

  cArray[58].name = "midnight blue";
  cArray[58].rgb[0] = 25;
  cArray[58].rgb[1] = 25;
  cArray[58].rgb[2] = 112;

  cArray[59].name = "dark blue";
  cArray[59].rgb[0] = 0;
  cArray[59].rgb[1] = 0;
  cArray[59].rgb[2] = 139;

  cArray[60].name = "medium blue";
  cArray[60].rgb[0] = 0;
  cArray[60].rgb[1] = 0;
  cArray[60].rgb[2] = 205;

  cArray[61].name = "royal blue";
  cArray[61].rgb[0] = 50;
  cArray[61].rgb[1] = 205;
  cArray[61].rgb[2] = 50;

  cArray[62].name = "blue violet";
  cArray[62].rgb[0] = 138;
  cArray[62].rgb[1] = 43;
  cArray[62].rgb[2] = 226;

  cArray[63].name = "indigo";
  cArray[63].rgb[0] = 75;
  cArray[63].rgb[1] = 0;
  cArray[63].rgb[2] = 130;

  cArray[64].name = "dark slate blue";
  cArray[64].rgb[0] = 72;
  cArray[64].rgb[1] = 61;
  cArray[64].rgb[2] = 139;

  cArray[65].name = "slate blue";
  cArray[65].rgb[0] = 106;
  cArray[65].rgb[1] = 90;
  cArray[65].rgb[2] = 205;

  cArray[66].name = "medium purple";
  cArray[66].rgb[0] = 147;
  cArray[66].rgb[1] = 112;
  cArray[66].rgb[2] = 219;

  cArray[67].name = "dark magenta";
  cArray[67].rgb[0] = 139;
  cArray[67].rgb[1] = 0;
  cArray[67].rgb[2] = 139;

  cArray[68].name = "royal blue";
  cArray[68].rgb[0] = 65;
  cArray[68].rgb[1] = 105;
  cArray[68].rgb[2] = 225;

  cArray[69].name = "dark violet";
  cArray[69].rgb[0] = 148;
  cArray[69].rgb[1] = 0;
  cArray[69].rgb[2] = 211;

  cArray[70].name = "purple";
  cArray[70].rgb[0] = 128;
  cArray[70].rgb[1] = 0;
  cArray[70].rgb[2] = 128;

  cArray[71].name = "thistle";
  cArray[71].rgb[0] = 216;
  cArray[71].rgb[1] = 191;
  cArray[71].rgb[2] = 216;

  cArray[72].name = "violet";
  cArray[72].rgb[0] = 238;
  cArray[72].rgb[1] = 130;
  cArray[72].rgb[2] = 238;

  cArray[73].name = "deep pink";
  cArray[73].rgb[0] = 50;
  cArray[73].rgb[1] = 205;
  cArray[73].rgb[2] = 50;

  cArray[74].name = "hot pink";
  cArray[74].rgb[0] = 255;
  cArray[74].rgb[1] = 105;
  cArray[74].rgb[2] = 180;

  cArray[75].name = "light pink";
  cArray[75].rgb[0] = 255;
  cArray[75].rgb[1] = 182;
  cArray[75].rgb[2] = 193;

  cArray[76].name = "pink";
  cArray[76].rgb[0] = 255;
  cArray[76].rgb[1] = 192;
  cArray[76].rgb[2] = 203;

  cArray[77].name = "wheat";
  cArray[77].rgb[0] = 245;
  cArray[77].rgb[1] = 222;
  cArray[77].rgb[2] = 179;

  cArray[78].name = "light yellow";
  cArray[78].rgb[0] = 255;
  cArray[78].rgb[1] = 255;
  cArray[78].rgb[2] = 224;

  cArray[79].name = "saddle brown";
  cArray[79].rgb[0] = 139;
  cArray[79].rgb[1] = 69;
  cArray[79].rgb[2] = 19;

  cArray[80].name = "chocolate";
  cArray[80].rgb[0] = 210;
  cArray[80].rgb[1] = 105;
  cArray[80].rgb[2] = 30;

  cArray[81].name = "sandy brown";
  cArray[81].rgb[0] = 244;
  cArray[81].rgb[1] = 164;
  cArray[81].rgb[2] = 96;
}
