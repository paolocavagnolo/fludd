#include <Controllino.h>

//OUTPUT
#define airPump_01 CONTROLLINO_R0   //pompa aria a 220V
#define ventoRX_02 CONTROLLINO_R1   //ventola per acquari a 220V
#define ventoSX_02 CONTROLLINO_R2   //ventola per acquari a 220V
#define oilP_A_03 CONTROLLINO_R3    //pompa per acquari a 220V
#define oilP_B_03 CONTROLLINO_R4    //pompa per acquari a 220V
#define oilP_C_03 CONTROLLINO_R5    //pompa per acquari a 220V

#define segnale CONTROLLINO_R8     //arduinoDUE con un servo

#define airPump_05 CONTROLLINO_R9   //pompa aria a 220V
#define ledCirc_06 CONTROLLINO_D1   //arduinoUNO con stripledRGB
#define ledDyn_45 CONTROLLINO_D2    //arduinoDUE con stripledRGB
#define ledDyn_67 CONTROLLINO_D3    //arduinoDUE con stripledRGB
#define dispencer_07a CONTROLLINO_R6
#define dispencer_07b CONTROLLINO_R7 //moteino segnale discesa 
#define ledBtn CONTROLLINO_D0       //led pulsante dispencer
#define sensV CONTROLLINO_D5

//INPUT
#define sensDisp CONTROLLINO_A0     //sensore di prossimita
#define btnDisp CONTROLLINO_A9   //pulsante discesa

//TEMPI
#define sprintTime 30    //100 secondi di sprint
#define erogazioneTime 10   //5 secondi di spegnimento led pulsante

uint8_t s = 0;
// 0 = idle
// 1 = erogazione (5 secondi dal pulsante premuto)
// 2 = sprint (100 secondi dal pulsante premuto)

unsigned long onTime = 0;

unsigned long t1 = 0;
unsigned long t2 = 0;
unsigned long t3 = 0;
unsigned long t4 = 0;

uint16_t f1 = 4;
uint16_t d1 = 500;

uint8_t f2 = 4;
uint16_t d2 = 5000;

uint8_t f3 = 4;
uint16_t d3 = 5000;
uint16_t xx = 3;
uint8_t oilP[] = {oilP_A_03,oilP_B_03,oilP_C_03,oilP_C_03,oilP_C_03};

uint16_t f4 = 4;
uint16_t d4 = 1000;

void setup() {
  pinMode(airPump_01,OUTPUT);
  pinMode(ventoRX_02,OUTPUT);
  pinMode(ventoSX_02,OUTPUT);
  pinMode(oilP_A_03,OUTPUT);
  pinMode(oilP_B_03,OUTPUT);
  pinMode(oilP_C_03,OUTPUT);
  pinMode(airPump_05,OUTPUT);
  pinMode(ledCirc_06,OUTPUT);
  pinMode(ledDyn_45,OUTPUT);
  pinMode(ledDyn_67,OUTPUT);
  pinMode(dispencer_07a,OUTPUT);
  pinMode(dispencer_07b,OUTPUT);
  digitalWrite(dispencer_07a, HIGH);
  digitalWrite(dispencer_07b, LOW);
  pinMode(ledBtn,OUTPUT);
  pinMode(sensV, OUTPUT);
  digitalWrite(sensV,HIGH);

  pinMode(segnale, OUTPUT);

  pinMode(sensDisp,INPUT);
  pinMode(btnDisp,INPUT);

  Serial.begin(9600);

  while(millis() < 60000) {
    digitalWrite(airPump_01, HIGH);
    digitalWrite(ventoRX_02, HIGH);
    digitalWrite(ventoSX_02, HIGH);
    digitalWrite(oilP_C_03, HIGH);
  }

  digitalWrite(airPump_01, LOW);
  digitalWrite(ventoRX_02, LOW);
  digitalWrite(ventoSX_02, LOW);
  digitalWrite(oilP_C_03, LOW);

}

unsigned long blinkLedTime = 0;
unsigned long pressedTime = 0;

void loop() {

  stampa_stato();

  if (s == 0) {
    //IDLE
    led_acceso();
    giochi_lenti();
    
    check_btn(); //to 1
    
  }
  else if (s == 1) {
    //EROGAZIONE
    led_blink();

    digitalWrite(airPump_01, HIGH);

    if ((millis() - pressedTime) > 5500) {
      digitalWrite(airPump_05, HIGH);
    }
    
    eroga(); //to 2 - inizia timeout
    
  }
  else if (s == 2) {
    //SPRINT
    led_acceso();
    giochi_veloci();
    check_btn(); //to 1
    check_timeout(); //to 0
    
  }
  else {
    //ERRORE
    led_spento();
  }

}

void stampa_stato() {
  if (millis()%1000 == 0) {
    Serial.print("Stato = ");
    Serial.println(s);
  }
}

void led_spento() {
  digitalWrite(ledBtn,LOW);
}

void led_acceso() {
  digitalWrite(ledBtn,HIGH);
}

void led_blink() {
  bool ledState = true;
  
  if ((millis() - blinkLedTime) > 100) {
    blinkLedTime = millis();
    ledState = !ledState;
    digitalWrite(ledBtn,ledState);
  }
}

void check_btn() {
  if (digitalRead(btnDisp) && s != 1) {
    s = 1;
    pressedTime = millis();
    spegni_tutto();
  }
}

void eroga() {
  if (((millis() - pressedTime) < 600)) {
    if(!digitalRead(sensDisp)) {
      digitalWrite(dispencer_07a,LOW);
      digitalWrite(dispencer_07b,HIGH);
    }

    digitalWrite(segnale,HIGH);
    
  }
  else if ((millis() - pressedTime) > erogazioneTime*1000) {
    s = 2;
    xx = 1;
    spegni_tutto();
  }
  else {
    digitalWrite(dispencer_07a,HIGH);
    digitalWrite(dispencer_07b,LOW);
    
    digitalWrite(segnale,LOW);
  }
  
}

void check_timeout() {
  Serial.println(millis() - pressedTime);
  if ((millis() - pressedTime) > sprintTime*1000) {
    s = 0;
    spegni_tutto();
  }
}


void spegni_tutto() {
  digitalWrite(airPump_01, LOW);

  digitalWrite(oilP_A_03, LOW);
  digitalWrite(oilP_B_03, LOW);
  digitalWrite(oilP_C_03, LOW);
  
  digitalWrite(ventoRX_02, LOW);
  digitalWrite(ventoSX_02, LOW);

  digitalWrite(airPump_05, LOW);
}




void giochi_lenti() {

  if ((millis() - t1) > f1*1000) {
    digitalWrite(airPump_01, HIGH);
    if ((millis() - t1) > f1*1000 + d1) {
      t1 = millis();
      digitalWrite(airPump_01, LOW);
      f1 = random(10,30);
      d1 = random(500,2000);
    }
  }

  
  if ((millis() - t2) > f2*1000) {
    digitalWrite(ventoRX_02, HIGH);
    digitalWrite(ventoSX_02, HIGH);
    if ((millis() - t2) > f2*1000 + d2) {
      t2 = millis();
      digitalWrite(ventoRX_02, LOW);
      digitalWrite(ventoSX_02, LOW);
      f2 = random(20,40);
      d2 = random(10000,20000);
    }
  }

  
  if ((millis() - t3) > f3*1000) {
    digitalWrite(oilP[xx], HIGH);
    if ((millis() - t3) > f3*1000 + d3) {
      t3 = millis();
      digitalWrite(oilP[xx], LOW);
      f3 = random(10,30);
      d3 = random(20000,40000);
      xx = random(5);
    }
  }

  if ((millis() - t4) > f4*1000) {
    digitalWrite(airPump_05, HIGH);
    if ((millis() - t4) > f4*1000 + d4) {
      t4 = millis();
      digitalWrite(airPump_05, LOW);
      f4 = random(5,20);
      d4 = random(500,2000);
    }
  }
  
}

void giochi_veloci() {

  if ((millis() - t1) > f1) {
    digitalWrite(airPump_01, HIGH);
    if ((millis() - t1) > f1 + d1) {
      t1 = millis();
      digitalWrite(airPump_01, LOW);
      f1 = random(1333);
      d1 = random(100,1000);
    }
  }

  
  digitalWrite(ventoRX_02, HIGH);
  digitalWrite(ventoSX_02, HIGH);


  if ((millis() - t3) > f3*1000) {
    digitalWrite(oilP[xx], HIGH);
    if ((millis() - t3) > f3*1000 + d3) {
      t3 = millis();
      digitalWrite(oilP[xx], LOW);
      f3 = 500;
      d3 = random(10000);
      xx = random(3);
    }
  }

  if ((millis() - t4) > f4) {
    digitalWrite(airPump_05, HIGH);
    if ((millis() - t4) > f4 + d4) {
      t4 = millis();
      digitalWrite(airPump_05, LOW);
      f4 = random(1500);
      d4 = random(50,200);
    }
  }
  
  
}
