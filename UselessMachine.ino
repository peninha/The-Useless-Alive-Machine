/* Máquina Inútil
 * Código para implementar a máquina inútil (que é sem graça, só uma máquina sem personalidade)
 * Autor: Roberto "Pena" Spinelli
 */
#include <Servo.h>
#define PinoBotao 2 //Pino do botão
#define PinoServo 8
#define upServo 150 //Posição do servo para Pressionar o botão
#define downServo 10  //Posição do servo para Dormir

Servo servo;  //Cria o objeto Servo (motor)

int botao; // Estado do botão

void setup() {
  servo.attach(PinoServo);  //configura o servo
  pinMode(PinoBotao, INPUT_PULLUP); //configura o botão
}

void loop() {
  botao = digitalRead(PinoBotao);
  if (botao==HIGH)
    servo.write(upServo);
  else{
    servo.write(downServo);
  }
}
