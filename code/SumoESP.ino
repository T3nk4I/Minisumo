#define LINE_A A1
#define LINE_B A0
#define RIGHT 2
#define FRONT_A 4
#define FRONT_B 7
#define LEFT A2
#define SERVO 10
#define SW1 1
#define SW2 12
#define REMOTE A5
#define Freq 1000
#define MRA_PWM 2
#define MRB_PWM 3
#define MLA_PWM 4
#define MLB_PWM 5

int MRightA = 6;
int MRightB = 3;
int MLeftA = 5;
int MLeftB = 9;

void setup() {
  pinMode(LINE_A, INPUT);
  pinMode(LINE_B, INPUT);
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
  pinMode(FRONT_A, INPUT);
  pinMode(FRONT_B, INPUT);
  pinMode(REMOTE, INPUT);
  pinMode(SW1, INPUT);
  pinMode(SW2, INPUT);
  pinMode(MRightA, OUTPUT);
  pinMode(MRightB, OUTPUT);
  pinMode(MLeftA, OUTPUT);
  pinMode(MLeftB, OUTPUT);
  ledcSetup(MRA_PWM, Freq, 8);
  ledcSetup(MRB_PWM, Freq, 8);
  ledcSetup(MLA_PWM, Freq, 8);
  ledcSetup(MLB_PWM, Freq, 8);
  ledcAttachPin(MRightA, MRA_PWM);
  ledcAttachPin(MRightB, MRB_PWM);
  ledcAttachPin(MLeftA, MLA_PWM);
  ledcAttachPin(MLeftB, MLB_PWM);
  Serial.begin(115200);
}

byte SensState(){
  bool sens1 = digitalRead(FRONT_A);
  byte sens2 = digitalRead(FRONT_B)*2;
  byte sens3 = digitalRead(RIGHT)*4;
  byte sens4 = digitalRead(LEFT) *8;
  byte total = sens1 + sens2 + sens3 + sens4;
  return total;
  Serial.println(total);
}

byte LineState(){
  bool line1 = digitalRead(LINE_A);
  byte line2 = digitalRead(LINE_B);
  byte total = line1 + line2;
  return total;
  Serial.println(total);
}

void adelante(byte PWM1, byte PWM2, int t){
  ledcWrite(MRA_PWM, PWM1);
  ledcWrite(MRB_PWM, 0);
  ledcWrite(MLA_PWM, PWM2);
  ledcWrite(MLB_PWM, 0);
  delay(t);
}

void atras(byte PWM1, byte PWM2, int t){
  ledcWrite(MRB_PWM, PWM1);
  ledcWrite(MRA_PWM, 0);
  ledcWrite(MLB_PWM, PWM2);
  ledcWrite(MLA_PWM, 0);
  delay(t);
}

void derecha(byte PWM1, byte PWM2, int t){
  ledcWrite(MRA_PWM, PWM1);
  ledcWrite(MRB_PWM, 0);
  ledcWrite(MLB_PWM, PWM2);
  ledcWrite(MLA_PWM, 0);
  delay(t);
}

void izquierda(byte PWM1, byte PWM2, int t){
  ledcWrite(MRB_PWM, PWM1);
  ledcWrite(MRA_PWM, 0);
  ledcWrite(MLA_PWM, PWM2);
  ledcWrite(MLB_PWM, 0);
  delay(t);
}

void paro(int t){
  ledcWrite(MRA_PWM, 255);
  ledcWrite(MRB_PWM, 255);
  ledcWrite(MLA_PWM, 255);
  ledcWrite(MLB_PWM, 255);
}

void paro(int t){
  ledcWrite(MRA_PWM, 0);
  ledcWrite(MRB_PWM, 0);
  ledcWrite(MLA_PWM, 0);
  ledcWrite(MLB_PWM, 0);
}

void loop() {
  while(digitalRead(REMOTE)==HIGH){
    switch (LineState()){
    case 0:
      Serial.println("BLANCO DOBLE");
      atras(200,200,40);
      izquierda(200,200,20);
      paro(100);
      break;
    case 1:
      Serial.println("BLANCO DERECHA");
      atras(200,200,40);
      derecha(200,200,20);
      break;
    case 2:
      Serial.println("BLANCO IZQUIERDA");
      atras(200,200,40);
      izquierda(200,200,20);
      break;
    case 3:
      Serial.println("NEGRO");
      switch (SensState()){
      case 1:
        Serial.println("CASO 1");
        derecha(80,80,10);
        break;
      case 2: //DETECTA SENSOR ENFRENTE IZQUIERDA.
        Serial.println("CASO 2");
        izquierda(80,80,10);
        break;
      case 3: //DETECTA DOBLE SENSOR ENFRENTE.
        Serial.println("CASO  3");
        adelante(80,80,10); //avanza con poca potencia
        delay(2);
        adelante(170,170,20); //avanza con media potencia
        delay(2);
        adelante(255,255,30); //avanza al full
        break;
      case 4: //DETECTA SOLO SENSOR DERECHO.
        Serial.println("CASO  4");
        derecha(255,255,10); //gira casi full a la derecha.
        break; 
      case 5: //DETECTA ENFRENTE DERECHA Y DERECHA.
        Serial.println("CASO  5");
        derecha(150,150,10); //gira con poca potencia a la derecha
        break;
      case 8: //DETECTA SOLO SENSOR IZQUIERDO
        Serial.println("CASO  8");
        izquierda(200,200,10); //gira casi full a la izquierda
        break;
      case 10:  //DETECTA ENFRENTE IZQUIERDA E IZQUIERDA.
        Serial.println("CASO  10");
        izquierda(150,150,10); //gira con poca potencia a la izquierda
        break;
      case 13:  //DETECTAN TODOS, 1vs10000 kmara qleros, manos les van a faltar para pelarme la verga.
        Serial.println("CASO Todos");
        derecha(255,255,10); //gira derecha full
        break;
      default:
        Serial.println("default");
        paro(5);
        adelante(50,50,10);
        break;
      }
      break;
    }
  }
}

