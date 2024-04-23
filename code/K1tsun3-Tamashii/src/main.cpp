#include <Arduino.h>
#include <Servo.h>

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

void paro(int t){
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

void adelante(int a, int b, int t){
  analogWrite(MA1, a);
  digitalWrite(MA2, LOW);
  analogWrite(MB1, b);
  digitalWrite(MB2, LOW);
  delay(t);
}

void atras(int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, 255);
  digitalWrite(MB1, LOW);
  analogWrite(MB2, 255);
  delay(t);
}

void izquierda(int a, int b, int t){
  analogWrite(MA1,a);
  digitalWrite(MA2, LOW);
  digitalWrite(MB1, LOW);
  analogWrite(MB2, b);
  delay(t);
}

void derecha(int a, int b, int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, a);
  analogWrite(MB1, b);
  digitalWrite(MB2, LOW);
  delay(t);
}

void loop() {
  adelante(255, 255, 1000);
  paro(100);
  atras(500);
}

