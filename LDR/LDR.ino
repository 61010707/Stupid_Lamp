#define ldr A0
#define R D2
#define G D5
#define B D6

int Rvalue = 0;
int Rvalues[10];
float Rvolt = 0.0;
int Gvalue = 0;
int Gvalues[10];
float Gvolt = 0.0;
int Bvalue = 0;
int Bvalues[10];
float Bvolt = 0.0;
int ldrValue = 0;
float volt = 0.0;
int value = 0.0;

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
  Serial.begin(115200);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(A0, INPUT);
  setRGB(1023, 1023, 1023);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  getColor();
}

void loop() {
//  ldrValue = avg();
//  volt = ldrValue * 5 / 1024;
//  Serial.print(volt);
//  Serial.print("  ");
//  test();
//  getColor();
  printValue();
}



int avg(){
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(ldr);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = total / numReadings;
  delay(1);
  return average;
}



float getColor(){
  setZero();
  for(int i = 0 ; i < 10 ; i++){
    analogWrite(R, 990);
    delay(10);
    Rvalues[i] = analogRead(ldr);
    delay(10);
    Rvalue += Rvalues[i];
  }
  Rvalue = Rvalue / 10;
  Rvolt = Rvalue * 5 / 1024;
  digitalWrite(R, HIGH);
  delay(100);

  for(int i = 0 ; i < 10 ; i++){
    analogWrite(G, 930);
    delay(10);
    Gvalues[i] = analogRead(ldr);
    delay(10);
    Gvalue += Gvalues[i];
  }
  Gvalue = Gvalue / 10;
  Gvolt = Gvalue * 5 / 1024;
  digitalWrite(G, HIGH);
  delay(100);

  for(int i = 0 ; i < 10 ; i++){
    analogWrite(B, 350);
    delay(10);
    Bvalues[i] = analogRead(ldr);
    delay(10);
    Bvalue += Bvalues[i];
  }
  Bvalue = Bvalue / 10;
  Bvolt = Bvalue * 5 / 1024;
  digitalWrite(B, HIGH);
  delay(100);

  setRGB(1023, 1023, 1023);
  delay(10);

  Rvalue = constrain(Rvalue, 120, 510);
  Rvalue = map(Rvalue, 120, 510, 1300, 650);

  Gvalue = constrain(Gvalue, 150, 510);
  Gvalue = map(Gvalue, 150, 510, 1300, 650);

  Bvalue = constrain(Bvalue, 150, 510);
  Bvalue = map(Bvalue, 150, 510, 1300, 650);
  
  setRGB(Rvalue, Gvalue, Bvalue);
}



void setZero(){
  Rvalue = 0;
  Gvalue = 0;
  Bvalue = 0;
  for (int i = 0 ; i < 10 ; i++){
    Rvalues[i] = 0;
    Gvalues[i] = 0;
    Bvalues[i] = 0;
  }
}



void setRGB(int r, int g, int b){
  analogWrite(R, r);
  analogWrite(G, g);
  analogWrite(B, b);
}


void printValue(){
  Serial.print("R=");
  Serial.print(Rvalue);
  Serial.print("  ");
  
  Serial.print("G=");
  Serial.print(Gvalue);
  Serial.print("  ");
  
  Serial.print("B=");
  Serial.println(Bvalue);
}

void test(){
      analogWrite(R, 1000);
    analogWrite(G, 1023);
    analogWrite(B, 1023);
    delay(500);

    analogWrite(R, 1023);
    analogWrite(G, 1023);
    analogWrite(B, 1023);
    delay(500);

    analogWrite(R, 1023);
    analogWrite(G, 1000);
    analogWrite(B, 1023);
    delay(500);
    
    analogWrite(R, 1023);
    analogWrite(G, 1023);
    analogWrite(B, 1023);
    delay(500);

    analogWrite(R, 1023);
    analogWrite(G, 1023);
    analogWrite(B, 1000);
    delay(500);
    
    analogWrite(R, 1023);
    analogWrite(G, 1023);
    analogWrite(B, 1023);
    delay(500);
}
