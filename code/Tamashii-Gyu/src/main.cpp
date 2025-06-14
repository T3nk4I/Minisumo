#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <avr/io.h>
#include <avr/interrupt.h>



#define SCREEN_WIDTH 128 //defines the width of the OLED screen
#define SCREEN_HEIGHT 64 //defines the height of the OLED screen
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); //sets up the OLED screen with the defined width and height

// defines sensor input pins
#define SENS0 8   //left diagonal sensor PB0
#define SENS1 9   //unused 
#define SENS2 12  //front left sensor PB4
#define SENS3 13  //right diagonal sensor PB5  
#define SENS4 A0  //front right sensor PC0
#define SENS5 A1  //unused  
#define SENS6 A2  //unused 
#define SENS7 A3  //middle line sensor PC3
#define SENS8 A6  // only analog inputs
#define SENS9 A7  // only analog inputs

// defines motor and servo output pins
#define MA1 11
#define MA2 6
#define MB2 3
#define MB1 5
#define SERVO 10
Servo FLAG;

// defines remote and button input pin
#define REMOTE 4  //PD4
#define BTN2 2    //PD2
#define BTN1 7    //PD7


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
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);

  pinMode(MA1, OUTPUT);
  pinMode(MA2, OUTPUT);
  pinMode(MB1, OUTPUT);
  pinMode(MB2, OUTPUT);
  FLAG.write(90);
  Serial.begin(9600);
}

void oledWrite(String current_mode) { //function to show on screen the current mode oled.clearDisplay();
  oled.setTextSize(3);
  oled.setCursor(0, 20);
  oled.setTextColor(WHITE);
  oled.println(current_mode);
  oled.display();
  //Serial.println(current_mode);
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

//byte sensval(){
  //bool FRONT_R = !digitalRead(SENS4);
  //byte FRONT_L = !digitalRead(SENS2) * 2;
  //byte RIGHT = digitalRead(SENS3) * 4;
  //byte LEFT = digitalRead(SENS0) *8;
  //byte total = FRONT_R + FRONT_L + RIGHT + LEFT;
  //return total;
//}

#define readSens(port, pin) ((port & (1 << pin)) != 0)

byte sensval(){
  return (!readSens(PORTC, PC0))^(!readSens(PORTB, PB4) << 1)^(readSens(PORTB, PB5) << 2)^(readSens(PORTB, PB0) << 3);
}

byte routines[] = {
  0,
  1,
  2
};

int main(){
  DDRB &= ~((1 << PB0) | (1 << PB4) | (1 << PB5)); // set SEN0, SENS2 and SENS3 as input
  DDRC &= ~((1 << PC0) | (1 << PC3)); // set SEN4 and SENS7 as input
  DDRD &= ~((1 << PD4) | (1 << PD2) | (1 << PD7)); // set REMOTE, BTN1 and BTN2 as input
  DDRB |= ((1 << PB1) | (1 << PB2)); // set MA1 and MA2 as output
  DDRD |= ((1 << PD3) | (1 << PD5)); // set MB2 and MB2 as output
  while (1) {
    off();
    FLAG.write(90); // set servo to neutral position
    if ((PORTD & (1 << PD2))) {
      _delay_ms(150); // increase delay for less sensitivity (longer press), decrease for more sensitivity (shorter press)
      rut++;
      rut > sizeof(routines) ? rut = 0 : rut; // reset to 0 if rut exceeds 15
    }
    if ((PORTD & (1 << PD7))) {
      _delay_ms(150);
      rut--;
      rut < 0 ? rut = sizeof(routines) : rut; // reset to 15 if rut is less than 0
    }
  }
  switch (rut) {
  case 0:
    
    break;
  
  }
  return 0;
}

void MainBattle() {
  while (digitalRead(REMOTE) == HIGH){
    FLAG.write(ang);
    if(digitalRead(SENS7)==1){
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
          case 8:
            right(50,50,20); 
            break;

          case 9:
            forwards(12,10,1);
            break;
          
          case 10:
            forwards(15,11,1);
            break;

          case 15:
            forwards(75,69,80);
            stop(70);
            break;

          default:
            forwards(100, 100,10);
            break;
        }
      }
    }
    else{
      backwards(175,175,100);
      right(255, 255, 175);
    }
  }
  if(digitalRead(REMOTE)==LOW){
    off();
  }
}

void rocket(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,255,200);
    ang = 180;
    MainBattle();
  }
  off();
}

void curveL(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(255,20,325);
    ang = 180;
    MainBattle();
  }
  off();
}

void curveR(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(20,255,325);
    ang = 0;
    MainBattle();
  }
  off();
}

void edgingL(){
  while (digitalRead(REMOTE)==HIGH){
    left(255,255,50);
    forwards(225,255,170);
    right(255,255,150);
    FLAG.write(0);
    ang = 0;
    stop(20);
    MainBattle();
  }
  off();
}

void edgingR(){
  while (digitalRead(REMOTE)==HIGH){
    right(255,255,60);
    forwards(225,255,170);
    left(255,255,150);
    FLAG.write(180);
    ang = 180;
    stop(20);
    MainBattle();
  }
  off();
}

void robotrick(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,255,125);
    left(255,255,220);
    FLAG.write(ang);
    MainBattle();
  }
  off();
}

void sideAttackR(){
  while (digitalRead(REMOTE)==HIGH){
    right(100, 100, 12);
    forwards(200,255,200);
    FLAG.write(180);
    left(255,255,180);
    MainBattle();
  }
}

void sideAttackL(){
  while (digitalRead(REMOTE)==HIGH){
    left(100, 100, 12);
    forwards(255,200,200);
    FLAG.write(0);
    right(255,255,180);
    MainBattle();
  }
}

void b2b(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,255,200);
    left(255,255,220);
    FLAG.write(ang);
    MainBattle();
  }
  off();
}

void s2s(){
  while (digitalRead(REMOTE)==HIGH){
    forwards(225,255,180);
    right(255,255,220);
    FLAG.write(ang);
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
      oledWrite(String(digitalRead(SENS7)));
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
    if(rut>=16){
      rut=1;
    }
  }
  if (digitalRead(BTN2)==HIGH){  //decrease routine cout if button 2 is pressed
    delay(150);
    rut--;
    Serial.println("Button 2 pressed");
    if(rut<=0){
      rut=15;
    }       
  }
  switch (rut){
    case 1: //Routine #1;
      oledWrite("Normal FR"); //works
      ang = 0;
      MainBattle();
      break;

    case 2:
      oledWrite("Normal FL"); //works
      ang = 180;
      MainBattle();
      break;

    case 3:
      oledWrite("Rocket");  //works
      rocket();
      break;
    
    case 4:
      oledWrite("Curve L"); //works
      ang = 180;
      curveL();
      break;

    case 5:
      oledWrite("Curve R"); //works
      ang = 0;
      curveR();
      break;

    case 6:
      oledWrite("Edging L");  //works
      edgingL();
      break;

    case 7:
      oledWrite("Edging R");  //works
      edgingR();
      break;

    case 8:
      oledWrite("Mexican"); //works
      MainBattle();
      break;

    case 9:
      oledWrite("Robotrick_R"); //works
      ang = 0;
      robotrick();
      break;
    
    case 10:
      oledWrite("Robotrick_L"); //works
      ang = 180;
      robotrick();
      break;

    case 11:
      oledWrite("Motor Test");
      motortest();
      break;
    
    case 12:
      oledWrite("Sensor Test");
      sensor_test();
      break;

    case 13:
      oledWrite("woodpecker");
      MainBattle();
      break;

    case 14:
      oledWrite("Side Attack R");
      sideAttackR();
      break;

    case 15:
      oledWrite("Side Attack L");
      sideAttackL();
      break;

    case 16:
      oledWrite("b2b");
      ang = 0;
      b2b();
      break;

    case 17:
      oledWrite("s2s");
      ang = 0;
      s2s();
      break;
  }
}