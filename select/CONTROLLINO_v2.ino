#include <Controllino.h>

#define airPump_01 CONTROLLINO_R0
#define ventoRX_02 CONTROLLINO_R1
#define sideLED CONTROLLINO_R2
#define oilP_A_03 CONTROLLINO_R3    //pompa per acquari a 220V
#define oilP_B_03 CONTROLLINO_R4    //pompa per acquari a 220V
#define oilP_C_03 CONTROLLINO_R5    //pompa per acquari a 220V
#define airPump_05 CONTROLLINO_R9
#define ledSignal CONTROLLINO_R8

#define ledBtn CONTROLLINO_D0       //led pulsante dispencer

#define dispencer_07a CONTROLLINO_R6
#define dispencer_07b CONTROLLINO_R7

#define btnDisp CONTROLLINO_A9   //pulsante discesa
#define sensBottle CONTROLLINO_A0


void setup() {
  pinMode(sideLED, OUTPUT);
  pinMode(airPump_01, OUTPUT);
  pinMode(ventoRX_02, OUTPUT);
  pinMode(oilP_A_03, OUTPUT);
  pinMode(oilP_B_03, OUTPUT);
  pinMode(oilP_C_03, OUTPUT);
  pinMode(airPump_05, OUTPUT);
  pinMode(ledSignal, OUTPUT);
  pinMode(ledBtn, OUTPUT);

  pinMode(dispencer_07a, OUTPUT);
  pinMode(dispencer_07b, OUTPUT);
  digitalWrite(dispencer_07a, HIGH);
  digitalWrite(dispencer_07b, LOW);

  digitalWrite(sideLED, HIGH);
  digitalWrite(ventoRX_02, HIGH);
  digitalWrite(oilP_A_03, LOW);
  digitalWrite(oilP_B_03, LOW);
  digitalWrite(oilP_C_03, LOW);
  digitalWrite(airPump_05, LOW);

  digitalWrite(ledBtn, HIGH);

}

unsigned long dPlay = 20000;

void loop() {


  if (digitalRead(btnDisp)) {
    dPlay = millis();
  }

  if ((millis() - dPlay) < 9000) {
    digitalWrite(sideLED, HIGH);
    vulcano();
    filtrazione();
    macerazione();
    ledDyn();
    eroga();
    ledBtnPulse();
  }
  else {
    spegni();
  }

  

}

unsigned long dv = 0, df = 0, dflag = 0, dpulse = 0;
uint8_t pins[3] = {oilP_A_03, oilP_B_03, oilP_C_03};
uint8_t i = 0;
uint8_t j = 0;
bool flagFilter = false;
bool dState = false;


void ledDyn() {

  if ((millis() - dPlay) < 500) {
    digitalWrite(ledSignal, HIGH);
  }
  else {
    digitalWrite(ledSignal, LOW);
  }


}

void spegni() {
  digitalWrite(sideLED, LOW);
  digitalWrite(ventoRX_02, HIGH);
  digitalWrite(oilP_A_03, LOW);
  digitalWrite(oilP_B_03, LOW);
  digitalWrite(oilP_C_03, LOW);
  digitalWrite(airPump_05, LOW);
  digitalWrite(ledBtn, HIGH);
}

void ledBtnPulse() {
  if ((millis() - dpulse) > 250) {
    dpulse = millis();
    dState = !dState;
    digitalWrite(ledBtn, dState);
  }
  
}

void macerazione() {

  digitalWrite(ventoRX_02, HIGH);

}

void filtrazione() {

  if (((millis() - dPlay) < 1000) || (((millis() - dPlay) > 5000) && ((millis() - dPlay) < 6000))) {
    digitalWrite(airPump_05, HIGH);
  }
  else {
    digitalWrite(airPump_05, LOW);
  }


}

void vulcano() {

  if ((millis() - dv) > random(5000, 15000)) {
    dv = millis();

    digitalWrite(pins[j], LOW);
    digitalWrite(pins[i], HIGH);

    j = i;
    i = random(3);
  }

}

void eroga() {

  //8.25 secondi stripled 
  //3.25 secondi caduta bottiglietta

  if (!digitalRead(A0)) {

    if (((millis() - dPlay) > 4500) && ((millis() - dPlay) < 5000) ) {
      digitalWrite(dispencer_07a, LOW);
      digitalWrite(dispencer_07b, HIGH);
    }
    else {
      digitalWrite(dispencer_07b, LOW);
      digitalWrite(dispencer_07a, HIGH);
    }

  }

}
