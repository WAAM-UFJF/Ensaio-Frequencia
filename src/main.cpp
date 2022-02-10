// Bibliotecas
#include <Arduino.h>
#include <Wire.h> 
#include <Adafruit_INA219.h>
#include <time.h>

// Arquivos
#include <encoder.h>
#include <confpwm.h>
#include <measurecurrent.h>
#include <measurevelocity.h>

#define SDA 21
#define SCL 22

// Definições do PWM e Ponte H

const int sentidoMotor1 = 2;  // Porta para definir o sentido de rotação 1.
const int sentidoMotor2 = 0;  // Porta para definir o sentido de rotação 2.

static int tempo_atual = 0, tempo_passado = 0;

extern int degrau;

void setup() {
  Serial.begin(115200);
  pinMode(sentidoMotor1, OUTPUT);
  pinMode(sentidoMotor2, OUTPUT);

  // Define como output os pinos que definem o sentido de rotação do motor
  digitalWrite(sentidoMotor1, LOW);
  digitalWrite(sentidoMotor2, HIGH);

  // Inicializa o sensor INA219
  inicializaINA();

  // Inicializa o PWM do motor com D = 0.6
  inicializaPWM();

  // Inicializa o encoder
  EncoderInit();

  // Altera PWM
  verificaPWM();

  // Senoide no PWM
  // xTaskCreatePinnedToCore(senoide, "loop2", 8192, NULL, 1, NULL, 0);
}



void loop() {
  tempo_passado = tempo_atual;
  // Faz a medição de velocidade
  measureVelocity();
  
  // Faz a medição de corrente com filtro de media movel
  measureCurrent();

  // Senoide
  senoide();


  Serial.println(degrau);
  tempo_atual = millis();

  // Serial.println(tempo_atual - tempo_passado);

}