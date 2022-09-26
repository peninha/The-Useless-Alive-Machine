/* Máquina Esperta
 * Código para implementar a máquina esperta (que é uma fofura, to apaixonado por ela, ela tá certamente viva)
 * Autor: Roberto "Pena" Spinelli
 */

#include <Servo.h>
#include <Gaussian.h>
#define PinoBotao 2 //Pino do botão
#define PinoServo 8 //Pino do motor
#define espiar 90 //Posição do servo para Espiar
#define ameacar 105 //Posição do servo para Ameaçar
#define pressionar 150 //Posição do servo para Pressionar o botão
#define dormir 10  //Posição do servo para Dormir
//Essas posições do servo podem mudar dependendo de como for a montagem da maquininha.
//Faça os ajustes conforme necessário

Servo servo;  //Cria o objeto Servo (motor)

int maquina = 0; //Estado da Máquina
int botao; //Estado do botão

/* ___Time = tempo desde que começou esse estado ___
   ___Base = tempo médio que deve ficar no estado ___
   ___Period = tempo que ele vai de fato ficar no estado ___
*/
unsigned long dormirTime = 0;
unsigned int dormirBase = 50000;
unsigned int dormirPeriod = dormirBase;
unsigned long ameacarTime = 0;
unsigned int ameacarBase = 1000;
unsigned int ameacarPeriod = ameacarBase;
unsigned long espiarTime = 0;
unsigned int espiarBase = 1800;
unsigned int espiarPeriod = espiarBase;
unsigned long ignorarTime = 0;
unsigned int ignorarBase = 2000;
unsigned int ignorarPeriod = ignorarBase;
unsigned int reboteBase = 800;
unsigned int rebotePeriod = reboteBase;
unsigned int pressionarChance = 650; // Qual a chance em 1000 da máquina querer apertar o botão
unsigned int ameacarChance = 850; // Qual a chance da máquina querer ameaçar apertar o botão
unsigned int reboteChance = 666; // Qual a chance em 1000 da máquina querer dar um rebote (apertar o botão após ameaçar)

float mean = 1; // valor médio da distribuição normal
float variance = 0.8; // variância da distribuição normal (mede quão espaçada é a distribuição)
Gaussian bellDistr = Gaussian(mean, variance);

void setup() {
  Serial.begin(9600); // prepara a porta serial
  randomSeed(analogRead(A0)); // gera uma semente para os números aleatórios
  //randomSeed(42);
  
  servo.attach(PinoServo); //configura o servo
  pinMode(PinoBotao, INPUT_PULLUP); //configura o botão
  attachInterrupt(digitalPinToInterrupt(PinoBotao), Click, HIGH);
  //servo.write(90);
}

void loop() { // rotina padrão
  botao = digitalRead(PinoBotao); // lê o estado do botão
  switch (maquina) { // direciona para a ação atual da máquina
    /* 0 = Dormindo
       1 = Pressionando
       2 = Ameaçando
       3 = Espiando
       4 = Blefando
    */
    case 1:
      Pressionar();
      break;
    case 2:
      Ameacar();
      break;
    case 3:
      Ignorar();
      break;
    case 4:
      Espiar();
      break;

    default: // por padrão dorme
      Dormir();
      break;
  }
}

void Click() { //Quando a máquina percebe que alguém apertou o botão
  Serial.println("click");
  int vontade = random(1001); //gera aleatoriamente a vontade da máquina
  if (vontade < pressionarChance) {
    vouPressionar();
  } else if (vontade < ameacarChance) {
    vouAmeacar();
  } else {
    vouIgnorar();
  }
}

void Dormir() {
  //Serial.println("DORMIR");
  servo.write(dormir);
  if (botao == HIGH) {
    if ((unsigned long)(millis() - dormirTime) > rebotePeriod) {
      Click();
    }
  } else {
    if ((unsigned long)(millis() - dormirTime) > dormirPeriod) {
      vouEspiar();
    }
  }
}

void Pressionar() {
  //Serial.println("PRESSIONAR");
  if (botao == LOW) {
    vouDormir();
  } else {
    servo.write(pressionar);
  }
}

void Ameacar() {
  //Serial.println("AMEACAR");
  if (botao == LOW) {
    vouDormir();
  } else {
    servo.write(ameacar);
    if ((unsigned long)(millis() - ameacarTime) > ameacarPeriod) {
      int rand = random(1001);
      if (rand > reboteChance) { // Rebotar
        vouDormir();
      } else {
        vouPressionar();
      }
    }
  }
}

void Espiar() {
  //Serial.println("ESPIAR");
  servo.write(espiar);
  if ((unsigned long)(millis() - espiarTime) > espiarPeriod) {
    vouDormir();
  }
}

void Ignorar() {
  //Serial.println("Ignorar");
  if ((unsigned long)(millis() - ignorarTime) > ignorarPeriod) {
    vouPressionar();
  }
}

void vouDormir() {
  Serial.println("vou Dormir");
  maquina = 0;
  dormirTime = millis();
  dormirPeriod = dormirBase * bellDistr.random();
  rebotePeriod = reboteBase * bellDistr.random();
  Serial.print("dormir periodo = ");
  Serial.println(dormirPeriod);
  Serial.print("rebote periodo = ");
  Serial.println(rebotePeriod);
}

void vouPressionar() {
  Serial.println("vou Pressionar");
  maquina = 1;
}

void vouAmeacar() {
  Serial.println("vou Ameacar");
  maquina = 2;
  ameacarTime = millis();
  ameacarPeriod = ameacarBase * bellDistr.random();
  Serial.print("periodo = ");
  Serial.println(ameacarPeriod);
}

void vouIgnorar() {
  Serial.println("vou Ignorar");
  maquina = 3;
  ignorarTime = millis();
  ignorarPeriod = ignorarBase * bellDistr.random();
  Serial.print("periodo = ");
  Serial.println(ignorarPeriod);
}

void vouEspiar() {
  Serial.println("vou Espiar");
  maquina = 4;
  espiarTime = millis();
  espiarPeriod = espiarBase * bellDistr.random();
  Serial.print("periodo = ");
  Serial.println(espiarPeriod);
}
