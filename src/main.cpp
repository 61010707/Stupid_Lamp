#include <Arduino.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
char auth[] = " b55b6f1b3a23440e97023a8584a9bde3";
char ssid[] = "DrZin";
char pass[] = "0985626152a";
int lightStatus = 0;
//*****************************//
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  lightStatus = pinValue;
}
void openLight()
{
  if (lightStatus == 1)
  {
    digitalWrite(D4, HIGH);
  }
  else
  {
    digitalWrite(D4, LOW);
  }
}
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  Serial.println(WiFi.localIP());
}

void loop()
{
  // put your main code here, to run repeatedly:
  Blynk.run();
  openLight();
}