#include <Arduino.h>

// Pino para alterar PWM
const int pinAlteraPWM = 15;

// Configurações PWM
const int motor  = 16;          // Define a porta de saída do sinal PWM.
int freq = 100;        // Define a frequencia a ser utilizada
const int motorChannel = 0;
int resolution = 8;           // Define a resolução que será utilizada no PWM.

int degrau = 0;

// Senoide
float T = 1/freq;
bool aux = 1;
int tempo_p = 0, tempo_a = 0;
int dT = 0;
int i = 0;

void alteraFreq(){
    if( freq < 20000){
        freq = 2*freq;
        ledcSetup(motorChannel,freq, resolution);
        ledcWrite(motorChannel, 0);
        T = 1/freq;
    }
    else{
        ledcSetup(motorChannel,40000, resolution);
        ledcWrite(motorChannel, 0);
        T = 1/40000;
    }
}

void verificaPWM()
{
    pinMode(pinAlteraPWM, INPUT);
    attachInterrupt(pinAlteraPWM, alteraFreq, RISING);
}

void inicializaPWM()
{
    pinMode(motor, OUTPUT);

    // Atribui o canal ao GPIO que será controlado
    ledcAttachPin(motor, motorChannel);

    // Configura o LED PWM
    ledcSetup(motorChannel, freq, resolution);  
    ledcWrite(motorChannel, 127);

}

void senoide()
{
    tempo_a = millis();
    dT = tempo_a - tempo_p;    
    if(aux == 1 && dT > T){
        i++;
        // Serial.println(i);
        ledcWrite(motorChannel, i);
        tempo_p = tempo_a;
    }

    if(aux == 0 && dT > T){
        i--;
        // Serial.println(i);
        ledcWrite(motorChannel, i);
        tempo_p = tempo_a;
    }



    if (i == 255 || i == 0){
        aux = !aux;

    }

    delay(10);
}