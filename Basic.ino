/*
This code is best used with the Tei board, which uses and Arduino Nano and a TB6612FNG H bridge
This code is meant to be used with digital IR sensors or similar digital sensors but can be modified to use more sensors and even a line sensor
*/

//defines sensors and PWM pins for easier coding
#define FRONT_R 2
#define FRONT_L 3
#define REMOTE A1
#define PWM_R 3
#define PWM_L 6

int MR_A 4;
int MR_B 7;
int ML_A 8;
int ML_B 11;


void setup() {
  pinMode(FRONT_R, INPUT);
  pinMode(FRONT_L, INPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  Serial.begin(9600);
}

//Function that converts the values of the opponent sensors into values from 0 to 3
void SensVal(){
  bool sens1 = digitalRead(FRONT_R);
  byte sens2 = digitalRead(FRONT_L) * 2;
  byte total = sens1 + sens2;
  return total;
}

//Function for motors
void Foward(byte a, byte b, int t){
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);
  analogWrite(PWM_R, a);
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);
  analogWrite(PWM_L, b);
  delay(t);
}

void Backwards(byte a, byte b, int t){
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);
  analogWrite(PWM_R, a);
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  analogWrite(PWM_L, b);
  delay(t);
}

void MOff() {
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, LOW);
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, LOW);
}

void Right(byte a, byte b, int t){
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);
  analogWrite(PWM_R, a);
  digitslWrite(ML_A, HIGH);
  gitialWrite(ML_B, LOW);
  analogWrite(PWM_L b);
  delay(t);
}

void Left(byte a, byte b, int t){
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);
  analogWrite(PWM_R, a);
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  analogWrite(PWM_L, b);
  delay(t);
}

void loop() {
  while(digitalRead(REMOTE)==HIGH){
    switch(SensVal()){
      case 1:
        Foward(65,100,50);
        break;
      case 2:
        Foward(100,65,50);
        break;
      case 3:
        Foward(255,255,60);
        break;
      default:
        Right(50,50,10);
        break;
    }
  }
  if(digitalRead(REMOTE)==LOW){
    MOff();
  }
}
