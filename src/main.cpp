// // Bibliotecas
#include <Arduino.h>
#include <Wire.h> 
#include <Adafruit_INA219.h>
#include <time.h>

// Arquivos
#include <encoder.h>
#include <confpwm.h>
#include <spwm.h>

#define SDA 21
#define SCL 22


// Definicao do sensor de corrente
Adafruit_INA219 ina219_0 (0x40);



// Definições do PWM e Ponte H
const int sentidoMotor1 = 2;  // Porta para definir o sentido de rotação 1.
const int sentidoMotor2 = 0;  // Porta para definir o sentido de rotação 2.

extern int degrau;

void inicializaINA(){
    while (1){
        if(ina219_0.begin()){
        break;
        }
        Serial.println("Falha ao encontrar o INA219");
        delay(20);
    }
    ina219_0.setCalibration_16V_400mA();
}


void setup() {
  Serial.begin(115200);
  pinMode(sentidoMotor1, OUTPUT);
  pinMode(sentidoMotor2, OUTPUT);

  // Define como output os pinos que definem o sentido de rotação do motor
  digitalWrite(sentidoMotor1, LOW);
  digitalWrite(sentidoMotor2, HIGH);

  // Inicializa o sensor INA219
  inicializaINA();

  // Inicializa o encoder
  EncoderInit();

  // Altera PWM
  // verificaPWM();

  // sPWM
  sPWM();


}



void loop() {


}



