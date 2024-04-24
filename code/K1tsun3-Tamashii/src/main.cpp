#include <Arduino.h>
#include <Servo.h>

// defines sensor input pins
#define SENS0 8
#define SENS1 11
#define SENS2 12
#define SENS3 13
#define SENS4 A0
#define SENS5 A1
#define SENS6 A4
#define SENS7 A5
#define SENS8 A6 // only analog inputs
#define SENS9 A7 // only analog inputs

// defines motor and servo output pins
#define MA1 10
#define MA2 9
#define MB1 5
#define MB2 6
#define SERVO 3

Servo FLAG;

void setup() {
  FLAG.attach(SERVO);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  FLAG.write(0);
  Serial.begin(9600);
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

void loop() {
  forwards(255, 255, 1000);
  stop(100);
  backwards(500);
}

