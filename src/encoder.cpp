#include <Arduino.h>


// Definições do Encoder
const int Encoder_C1 = 12;
const int Encoder_C2 = 14;
byte Encoder_C1Last;
int duracao;
boolean Direcao;
int resolucao = 0;

int tempo_atual = 0, tempo_passado = 0;
int deltaT;
int pulso = 0;

void calculapulso()
{
  int Lstate = digitalRead(Encoder_C1);
  if ((Encoder_C1Last == LOW) && Lstate == HIGH)
  {
    int val = digitalRead(Encoder_C2);
    if (val == LOW && Direcao)
    {
      Direcao = false; //Reverse
    }
    else if (val == HIGH && !Direcao)
    {
      Direcao = true;  //Forward
    }
  }
  Encoder_C1Last = Lstate;
  if (!Direcao)  duracao++;
  else  duracao--;

  pulso++;
  if(pulso > 50){
    tempo_passado = tempo_atual;
    tempo_atual = millis();
    deltaT = tempo_atual - tempo_passado;
    pulso = 0;
    // Serial.println("Encoder funcionando!");
  }
}

void calculaResolucao(){
  resolucao++;
  Serial.print("Numero de bordas de subida: ");
  Serial.println(resolucao);
}


void EncoderInit()
{
  //Serial.println("Iniciei o Encoder");
  pinMode(Encoder_C2, INPUT);
  attachInterrupt(Encoder_C2, calculapulso, RISING);
  //attachInterrupt(14, calculaResolucao, RISING);
}
