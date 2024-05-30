#include <Arduino.h>
#define LINE 11
#define FRONT_R 7
#define FRONT_L 12
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
  int sens1 = analogRead(FRONT_R);
  int sens2 = analogRead(FRONT_L);
  if (sens1 >= x || sens2 <= x){
    return 1;
  }
  else if (sens1 <= x || sens2 >= x){
    return 2;
  }
  else if (sens1 >= x || sens2 >= x){
    return 3;
  }
  else{
    return 0;
  }
}

void loop() {
  while(digitalRead(REMOTE)==HIGH){
    while (digitalRead(LINE)==LOW){
      switch (sensval(1000)){
        case 1:
          //right
          break;
        
        case 2:
          //left
          break;
        
        case 3:
          //forwards
          break;
    
        default:
          break;
      }
    } 
    if(digitalRead(LINE)==HIGH){
      //backwards
      //turn
      //forwards
    }
  }
  if (digitalRead(REMOTE)==LOW){
    //off
  }
  
}