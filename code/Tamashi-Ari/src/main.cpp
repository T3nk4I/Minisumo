#include <Arduino.h>
#define LINE A3 
#define FRONT_R A7
#define FRONT_L A0 
#define REMOTE 2

#define MA1 6
#define MA2 3
#define MB1 5
#define MB2 9

void setup() {
  pinMode(LINE, INPUT);
  pinMode(FRONT_R, INPUT);
  pinMode(FRONT_L, INPUT);
  pinMode(REMOTE, INPUT);

  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);

  Serial.begin(9600);
}

byte sensval(int x){ 
  /*
  using Sharp sharp gp2y0e03 sensors values are from 0 to aprox. 500
  0 is furthest and 500 closest, but once it reaches 4cm range the values are between 5-15
  */
  int sens1 = analogRead(FRONT_R);
  int sens2 = analogRead(FRONT_L);
  if (sens1 >= x && sens2 <= x){
    return 1;
  }
  else if (sens1 <= x && sens2 >= x){
    return 2;
  }
  else if (sens1 >= x && sens2 >= x){
    return 3;
  }
  else{
    return 0;
  }
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
  analogWrite(MB2, b);
  digitalWrite(MB1, LOW);
  delay(t);
}

void backwards(int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, 255);
  digitalWrite(MB2, LOW);
  analogWrite(MB1, 255);
  delay(t);
}

void left(int a, int b, int t){
  analogWrite(MA1,a);
  digitalWrite(MA2, LOW);
  digitalWrite(MB2, LOW);
  analogWrite(MB1, b);
  delay(t);
}

void right(int a, int b, int t){
  digitalWrite(MA1, LOW);
  analogWrite(MA2, a);
  analogWrite(MB2, b);
  digitalWrite(MB1, LOW);
  delay(t);
}

void loop() { 
  int Y = 400;
  while (digitalRead(LINE)==HIGH){
  //Serial.println(sensval(Y));
  //Serial.println(Y);
    switch (sensval(Y)){
    case 1: 
      forwards(80,100,20);
      Y=400;
      break;
    
    case 2:
      forwards(100,80,20);
      Y=400;
      break;

    case 3:
      forwards(155,155,50);
      Y=0;
      break;

    default:
      left(30,30,20);
      Y=400;
      break;
    }
  }
  if(digitalRead(LINE)==LOW){
    stop(10);
    backwards(100);
    right(30,30,50);
  }
}