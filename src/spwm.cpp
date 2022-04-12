#include <Arduino.h>
#include <measurecurrent.h>
#include <measurevelocity.h>

#define Num_Samples  112

#define PIN_PWM 16                // Porta de saída para o sPWM que alimenta a ponte-h.
#define PIN_MEAS 32               // Define a porta que gera o sinal PWM no qual a interrupção  de leitura está alocada.

int freq_spwm = 1000;
int index_spwm = 0;
static int canal = 0 , canal_meas = 1;

double t1, t2;

/* create a hardware timer */
hw_timer_t * timer = NULL;
hw_timer_t * measureDatasTimer = NULL;
TaskHandle_t measureDatas;

const unsigned char WaveFormTable[Num_Samples] = {
    0x80, 0x83, 0x87, 0x8A, 0x8E, 0x91, 0x95, 0x98, 0x9B, 0x9E, 0xA2, 0xA5, 0xA7, 0xAA, 0xAD, 0xAF,
    0xB2, 0xB4, 0xB6, 0xB8, 0xB9, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xBF, 0xBF, 0xC0, 0xBF, 0xBF, 0xBF,
    0xBE, 0xBD, 0xBC, 0xBB, 0xB9, 0xB8, 0xB6, 0xB4, 0xB2, 0xAF, 0xAD, 0xAA, 0xA7, 0xA5, 0xA2, 0x9E,
    0x9B, 0x98, 0x95, 0x91, 0x8E, 0x8A, 0x87, 0x83, 0x80, 0x7C, 0x78, 0x75, 0x71, 0x6E, 0x6A, 0x67,
    0x64, 0x61, 0x5D, 0x5A, 0x58, 0x55, 0x52, 0x50, 0x4D, 0x4B, 0x49, 0x47, 0x46, 0x44, 0x43, 0x42,
    0x41, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x41, 0x42, 0x43, 0x44, 0x46, 0x47, 0x49, 0x4B,
    0x4D, 0x50, 0x52, 0x55, 0x58, 0x5A, 0x5D, 0x61, 0x64, 0x67, 0x6A, 0x6E, 0x71, 0x75, 0x78, 0x7C
};


void IRAM_ATTR onTimer(){  
  ledcWrite(canal, WaveFormTable[index_spwm]);
  index_spwm++;
  if(index_spwm >= Num_Samples)
    index_spwm = 0;
}

// void IRAM_ATTR onMeasureDatas(){  
//   measureCurrent();
//   measureVelocity();
// }


// void measureDatasFunction( void * pvParameters ){
//   measureDatas = timerBegin(1, 800, true);
//   Serial.println("Estou em measureDatasFunction!");
//   timerAttachInterrupt(measureDatasTimer, &onMeasureDatas, true);
//   Serial.println("Estou em measureDatasFunction após dar o attach na interrupt!");
//   timerAlarmWrite(measureDatasTimer, 10000000 * (1.0/(freq_spwm*Num_Samples)), true);
//   timerAlarmEnable(measureDatasTimer);
//   while(1){

//   }    
// }


void onMeasureDatas(){
  // measureCurrent();
  // measureVelocity();
  // Serial.println("Entrei no onMeasureDatas!");
  Serial.println(xPortGetCoreID());
}

void measureDatasFunction( void * pvParameters ){
  // pinMode(PIN_MEAS, OUTPUT);
  // ledcAttachPin(PIN_MEAS, canal_meas);
  // ledcSetup(canal_meas, 900, 8);
  // ledcWrite(canal_meas, 128);
  // attachInterrupt(PIN_MEAS, onMeasureDatas, RISING);
  // // vTaskDelete(measureDatas);
  while(1){
    t1 = micros();
    measureCurrent();
    measureVelocity();
    t2 = micros();
    Serial.println(t2-t1);
  }
}


void sPWM() {
  pinMode(PIN_PWM, OUTPUT);
  ledcAttachPin(PIN_PWM, canal);
  ledcSetup(canal, freq_spwm, 8);
  ledcWrite(canal, 0);

  /* Use 1st timer of 4 */
  /* 1 tick take 1/(80MHZ/800) = 0.1us so we set divider 800 and count up */
  timer = timerBegin(0, 800, true);


  /* Attach onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer, true);
  

  /* Set alarm to call onTimer function every second 1 tick is 0.1us
  => 1 second is 1000000us */
  /* Repeat the alarm (third parameter) */
  timerAlarmWrite(timer, 10000000 * (1.0/(freq_spwm*Num_Samples)), true);
  

  /* Start an alarm */
  timerAlarmEnable(timer);
  
  Serial.println("start timer");

  xTaskCreatePinnedToCore(
                  measureDatasFunction,   /* Task function. */
                  "measureDatas",         /* name of task. */
                  10000,                  /* Stack size of task */
                  NULL,                   /* parameter of the task */
                  1,                      /* priority of the task */
                  &measureDatas,          /* Task handle to keep track of created task */
                  0);   
}


