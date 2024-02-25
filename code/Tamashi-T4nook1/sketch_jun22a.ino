#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#define LINE_R 8
#define LINE_L 11
#define FRONT_R 7
#define FRONT_L 12
#define RIGHT 4
#define LEFT 13
#define SW1 1
#define SW2 12
#define REMOTE 2
#define SERVO 10

#define MA1 6
#define MA2 3
#define MB1 5
#define MB2 9

Servo FLAG; 

byte RUTINA = 0;

void setup() { 
  FLAG.attach(SERVO);
  pinMode(LINE_R, INPUT);
  pinMode(LINE_L, INPUT);
  pinMode(FRONT_R, INPUT);
  pinMode(FRONT_L, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(REMOTE, INPUT);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  FLAG.write(0);
  Serial.begin(9600);
}

byte sensval(){
  bool sens1 = !digitalRead(FRONT_R);
  byte sens2 = !digitalRead(FRONT_L)*2;
  byte sens3 = digitalRead(RIGHT)*4;
  byte sens4 = digitalRead(LEFT)*8;
  byte total = sens1 + sens2 + sens3 + sens4;
  return total;
}

byte lineval(){
  bool line1 = digitalRead(LINE_R);
  byte line2 = digitalRead(LINE_L)*2;
  byte total = line1 + line2;
  return total; //3 NEGRO, 2 BLANCO IZQUIERDO, 1 BLANCO DERECHO, 0 BLANCO
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

void loop(){
  while(digitalRead(REMOTE)==HIGH){
    FLAG.write(90);
    switch(lineval()){
      case 0: //BLANCO
        paro(75);
        atras(175);
        derecha(255,255,175);
        break;
      case 1: //IZQUIERDA
        paro(10);
        atras(150);
        derecha(255,255,100);
        break;
      case 2: //DERECHA
        paro(10);
        atras(150);
        izquierda(255,255,100);
        break;
      case 3: //NEGRO
          switch(sensval()){
            case 1:
              adelante(75,100,20);
              break;
            case 2:
              adelante(100,75,20);
              break;
            case 3:
              adelante(255,255,30);
              break;
            case 4:
              derecha(255,255,20);
              break;
            case 5:
              derecha(255,255,15);
              break;
            case 7:
              derecha(255,255,50);
              break;
            case 8:
              izquierda(255,255,20);
              break;
            case 10:
              izquierda(255,255,15);
              break;
            case 11:
              izquierda(255,255,50);
              break;
            case 15:
              adelante(175,175,30);
              break;
            default:
              adelante(25,15,5);
              break;
          }
        break;
    }
  }
  if(digitalRead(REMOTE)==LOW){
    off();
    FLAG.write(0);
  }
}
