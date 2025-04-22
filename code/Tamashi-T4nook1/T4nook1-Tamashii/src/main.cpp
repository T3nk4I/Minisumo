#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 //defines the width of the OLED screen
#define SCREEN_HEIGHT 64 //defines the height of the OLED screen
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //sets up the OLED screen with the defined width and height

// defines sensor input pins
#define SENS0 8 // line sensor LEFT
#define SENS1 9  //left diagonal sensor
#define SENS2 12  //front left sensor
#define SENS3 13  //line sensor RIGHT 
#define SENS4 A0  //right diagonal sensor
#define SENS5 A1  //front right sensor
#define SENS6 A2  
#define SENS7 A3  
#define SENS8 A6 // only analog inputs
#define SENS9 A7 // only analog inputs

// defines motor and servo output pins
#define MA1 6
#define MA2 11
#define MB2 3
#define MB1 5
#define SERVO 10
Servo FLAG;

// defines remote and button input pin
#define REMOTE 4
#define BTN2 2
#define BTN1 7

byte rut = 1;
bool test_mode;
int ang;

void setup() {
  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3c)){
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }
  delay(2000);
  oled.clearDisplay();
  oled.setCursor(0, 0);
  FLAG.attach(SERVO);
  pinMode(SENS0, INPUT);
  pinMode(SENS1, INPUT);
  pinMode(SENS2, INPUT);
  pinMode(SENS3, INPUT);
  pinMode(SENS4, INPUT);
  pinMode(SENS5, INPUT);
  pinMode(SENS6, INPUT);
  pinMode(SENS7, INPUT);
  pinMode(SENS8, INPUT); 
  pinMode(SENS9, INPUT);

  pinMode(REMOTE, INPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN2, INPUT_PULLUP);

  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  FLAG.write(90);
  Serial.begin(9600);
}

void oledWrite(String current_mode) { //function to show on screen the current mode
  oled.clearDisplay();
  oled.setTextSize(3);
  oled.setCursor(0, 20);
  oled.setTextColor(WHITE);
  oled.println(current_mode);
  oled.display();
  Serial.println(current_mode);
}

void stop(int t){
  analogWrite(MA1,255);
  analogWrite(MA2,255);
  analogWrite(MB1,255);
  analogWrite(MB2,255);
  delay(t);
}

void off(){
  analogWrite(MA1,0);
  analogWrite(MA2,0);
  analogWrite(MB1,0);
  analogWrite(MB2,0);
}

void forwards(int a, int b, int t){
  analogWrite(MA1, a);
  digitalWrite(MA2, LOW);
  analogWrite(MB1, b);
  digitalWrite(MB2, LOW);
  delay(t);
}

void backwards(int a, int b, int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, a);
  digitalWrite(MB1, LOW);
  analogWrite(MB2, b);
  delay(t);
}

void left(int a, int b, int t){
  analogWrite(MA1,a);
  digitalWrite(MA2, LOW);
  digitalWrite(MB1, LOW);
  analogWrite(MB2, b);
  delay(t);
}

void right(int a, int b, int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, a);
  analogWrite(MB1, b);
  digitalWrite(MB2, LOW);
  delay(t);
}

byte sensval(){
  bool FRONT_R = !digitalRead(SENS3);
  byte FRONT_L = !digitalRead(SENS0) * 2;
  byte RIGHT = digitalRead(SENS5) * 4;
  byte LEFT = digitalRead(SENS2) *8;
  byte total = FRONT_R + FRONT_L + RIGHT + LEFT;
  return total;
}

byte lineval(){
  bool LINE_R = digitalRead(SENS4);
  byte LINE_L = digitalRead(SENS1) * 2;
  byte total = LINE_R + LINE_L;
  return total;
}

void MainBattle() {
  while (digitalRead(REMOTE) == HIGH){
    FLAG.write(180);
    switch (lineval()){
    case 0:
      backwards(175,175,100);
      right(255, 255, 175);
      break;
    
    case 1:
      backwards(175,175,70);
      right(255, 255, 100);
      break;

    case 2:
      backwards(175,175,70);
      left(255, 255, 100);
      break;

    case 3:
      switch (sensval()){
        case 1:
          forwards(75,100,20);
          break;

        case 2:
          forwards(100,75,20);
          break;

        case 3:
          forwards(255,255,30);
          break;

        case 4:
          right(255,255,20);
          break;

        case 5:
          right(255,255,15);
          break;

        case 7:
          right(255,255,50);
          break;
          
        case 8:
          left(255,255,20);
          break;

        case 10:
          left(255,255,15);
          break;

        case 11:
          left(255,255,50);
          break;

        case 15:
          forwards(175,175,30);
          break;

        default:
          switch (rut){
          case 7:
            right(50,50,20); 
            break;
          
          default:
            forwards(40,25,80);
            delay(200);
            break;
          }
      }
      break;
    }
  }
  if (digitalRead(REMOTE)==LOW){
    off();
  }
}

void rocket(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,255,200);
    MainBattle();
  }
  off();
}

void curveL(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,25,400);
    MainBattle();
  }
  off();
}

void curveR(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(25,225,400);
    MainBattle();
  }
  off();
}

void edgingL(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(175,175,275);
    right(255,255,150);
    MainBattle();
  }
  off();
}

void edgingR(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(175,175,275);
    left(255,255,150);
    MainBattle();
  }
  off();
}

void motortest(){
  while (digitalRead(REMOTE)==HIGH){
    for(int i = 0; i<=255; i++){
      forwards(i,i,10);
    }
    for(int i = 0; i<=255; i++){
      backwards(i,i,10);
    }
    for(int i = 0; i<=255; i++){
      right(i,i,10);
    }
    for(int i = 0; i<=255; i++){
      left(i,i,10);
    }
  }
  
}

void sensor_test(){
  while (digitalRead(REMOTE)==HIGH){
    if(digitalRead(BTN1)==HIGH){
      test_mode = true;
    }
    if(digitalRead(BTN2)==HIGH){
      test_mode = false;
    }
    switch (test_mode){
    case 0:
      oledWrite(String(sensval()));
      break;
    
    case 1:
      oledWrite(String(lineval()));
      break;
    }
  }
}

void loop(){
  off();
  FLAG.write(90);
  if (digitalRead(BTN1)==HIGH){  //increase routine cout if button 1 is pressed
    delay(150); //increase delay for less sensitivity (longer press), decrease for more sensitivity (shorter press)
    rut++;
    Serial.println("Button 1 pressed");
    if(rut>=10){
      rut=0;
    }
  }
  if (digitalRead(BTN2)==HIGH){  //decrease routine cout if button 2 is pressed
    delay(150);
    rut--;
    Serial.println("Button 2 pressed");
    if(rut<=0){
      rut=8;
    }       
  }
  switch (rut){
    case 1: //Routine #1;
      oledWrite("Normal");
      MainBattle();
      break;
    
    case 2:
      oledWrite("Rocket");
      rocket();
      break;
    
    case 3:
      oledWrite("Curve L");
      curveL();
      break;

    case 4:
      oledWrite("Curve R");
      curveR();
      break;

    case 5:
      oledWrite("Edging L");
      edgingL();
      break;

    case 6:
      oledWrite("Edging R");
      edgingR();
      break;

    case 7:
      oledWrite("Mexican");
      MainBattle();
      break;
    
    case 8:
      oledWrite("Motor Test");
      motortest();
      break;
    
    case 9:
      oledWrite("Sensor Test");
      sensor_test();
      break;

    case 10:
      oledWrite("self_destruct");
      break;
  }
}