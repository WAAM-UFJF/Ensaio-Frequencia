// // Bibliotecas
#include <Arduino.h>
#include <Wire.h> 
#include <Adafruit_INA219.h>
#include <time.h>

// Arquivos
#include <encoder.h>
#include <confpwm.h>
#include <measurecurrent.h>
#include <measurevelocity.h>
#include <spwm.h>

#define SDA 21
#define SCL 22


TaskHandle_t measureDatas;


void measureDatasFunction( void * pvParameters ){
  while(1){
    measureCurrent();
    measureVelocity();
  }
    
}
// Definições do PWM e Ponte H

const int sentidoMotor1 = 2;  // Porta para definir o sentido de rotação 1.
const int sentidoMotor2 = 0;  // Porta para definir o sentido de rotação 2.

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
  // inicializaPWM();

  // Inicializa o encoder
  EncoderInit();

  // Altera PWM
  // verificaPWM();

  // sPWM
  sPWM();

    xTaskCreatePinnedToCore(
                    measureDatasFunction,         /* Task function. */
                    "measureDatas",       /* name of task. */
                    10000,                /* Stack size of task */
                    NULL,                 /* parameter of the task */
                    1,                    /* priority of the task */
                    &measureDatas,        /* Task handle to keep track of created task */
                    0);   
}



void loop() {
  // // Faz a medição de velocidade
  // measureVelocity();
  
  // // Faz a medição de corrente com filtro de media movel
  // measureCurrent();

  // Serial.println(" ");

  // Serial.println(degrau);

}

