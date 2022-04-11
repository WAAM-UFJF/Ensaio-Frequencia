#include <Arduino.h>
#include <Adafruit_INA219.h>

// Definição do sensor de corrente e tensão.
Adafruit_INA219 ina219_0 (0x40);

// Define o valor de amostras para a media movel
static const int N = 128;
static const float n = 1.0/N;
static float mediaMovel[N];
static int contador=0;


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


void measureCurrent(){
    float corrente = 0, correnteFiltrada = 0;
    contador++;

    corrente = ina219_0.getCurrent_mA();
    mediaMovel[(contador-1)%N] = corrente;

    if(contador < N){
        for(int i=0; i<contador+1;i++){
            correnteFiltrada += mediaMovel[i];
        }
        correnteFiltrada = correnteFiltrada/contador;
    }
    else{
        for(int i=0; i<N; i++){
            correnteFiltrada += mediaMovel[i];
        }
        correnteFiltrada = correnteFiltrada*n;    
    }
    Serial.print(correnteFiltrada);
    Serial.print(";");
}