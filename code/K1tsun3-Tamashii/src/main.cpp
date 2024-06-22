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
#define MA1 11
#define MA2 6
#define MB1 3
#define MB2 5
#define SERVO 10
Servo FLAG;

// defines remote and button input pin
#define REMOTE 4
#define BTN1 2
#define BTN2 7

byte rut;

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
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  FLAG.write(0);
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

void backwards(int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, 255);
  digitalWrite(MB1, LOW);
  analogWrite(MB2, 255);
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
  bool FRONT_R = digitalRead(SENS5);
  byte FRONT_L = digitalRead(SENS2) * 2;
  byte RIGHT = digitalRead(SENS3) * 4;
  byte LEFT = digitalRead(SENS1) *8;
  byte total = FRONT_R + FRONT_L + RIGHT + LEFT;
  return total;
}

byte lineval(){
  bool LINE_R = digitalRead(SENS3);
  byte LINE_L = digitalRead(SENS0) * 2;
  byte total = LINE_R + LINE_L;
  return total;
}

void MainBattle() {
  while (digitalRead(REMOTE) == HIGH){
    switch (sensval()){
    case 0:
      stop(75);
      backwards(175);
      right(255, 255, 175);
      break;
    
    case 1:
      stop(25);
      backwards(150);
      right(255, 255, 100);
      break;

    case 2:
      stop(25);
      backwards(150);
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
          forwards(15,15,5);
      }
      break;
    }
  }
  if (digitalRead(REMOTE)==LOW){
    off();
  }
}

void loop(){
  /*
  byte totalRoutines = 18; //total of all the routines, change to increase or decrease routine numbers
  if (digitalRead(BTN1) == HIGH){
    delay(200);
    Serial.println("Button 1 pressed");
  }
  else{
    Serial.println("Button 1 not pressed");
  }
  
  while(rut >= totalRoutines || rut <= 0){
    if (digitalRead(BTN1)==HIGH){  //increase routine cout if button 1 is pressed
      delay(150); //increase delay for less sensitivity (longer press), decrease for more sensitivity (shorter press)
      rut++;
      Serial.println("Button 1 pressed");
    }
    if (digitalRead(BTN2)==HIGH){  //decrease routine cout if button 2 is pressed
      delay(150);
      rut--;
      Serial.println("Button 1 pressed");
    }
    switch (rut){
      case 0: //Routine #1;
        oledWrite("Normal");
        break;
      
      case 1:
        oledWrite("Rocket");
        break;
      
      case 2:
        oledWrite("Curve L");
        break;

      case 3:
        oledWrite("Curve R");
        break;

      case 4:
        oledWrite("Edging L");
        break;

      case 5:
        oledWrite("Edging R");
        break;
      
      case 6:
        oledWrite("Motor Test");
        break;
      
      case 7:
        oledWrite("Sensor Test");
        break;
    }
  }
  */
}