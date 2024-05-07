//CODICE GIOCO TOTALE
//CONTROLLINO MAXI
//Caricato il 16/12/2022

#include <Controllino.h>

#define airPump_01 CONTROLLINO_R0
#define ventoRX_02 CONTROLLINO_R1
#define sideLED CONTROLLINO_R2
#define oilP_A_03 CONTROLLINO_R3   //pompa per acquari a 220V
#define oilP_BC_03 CONTROLLINO_R4  //pompa per acquari a 220V
#define blending_04 CONTROLLINO_R5
#define airPump_05 CONTROLLINO_R9
#define ledSignal CONTROLLINO_R8

#define ledBtn CONTROLLINO_D0  //led pulsante dispencer

#define dispencer_07 CONTROLLINO_R7

#define btnDisp CONTROLLINO_A9  //pulsante discesa
#define sensBottle CONTROLLINO_A0
#define sensPresenza CONTROLLINO_A1  //sensore Presenza


void setup() {
  pinMode(sideLED, OUTPUT);
  pinMode(airPump_01, OUTPUT);
  pinMode(ventoRX_02, OUTPUT);
  pinMode(oilP_A_03, OUTPUT);
  pinMode(oilP_BC_03, OUTPUT);
  pinMode(airPump_05, OUTPUT);
  pinMode(ledSignal, OUTPUT);
  pinMode(ledBtn, OUTPUT);
  pinMode(blending_04, OUTPUT);

  pinMode(dispencer_07, OUTPUT);
  digitalWrite(dispencer_07, LOW);

  digitalWrite(airPump_01, LOW);
  digitalWrite(sideLED, LOW);
  digitalWrite(blending_04, LOW);
  digitalWrite(ventoRX_02, LOW);
  digitalWrite(oilP_A_03, LOW);
  digitalWrite(oilP_BC_03, LOW);
  digitalWrite(airPump_05, LOW);

  digitalWrite(ledBtn, LOW);

  while (millis() < 60000) {
    digitalWrite(airPump_01, HIGH);
    digitalWrite(ventoRX_02, HIGH);
  }

  digitalWrite(airPump_01, LOW);

  //ALL READY!
  digitalWrite(ledBtn, HIGH);
}

unsigned long dPlay = 20000, tStall = 0;
uint8_t sPlay = 0;
int i = 0;

bool bottone = false;
bool presenza = false;
int bottoneCounter = 0;
int presenzaCounter = 0;
unsigned long presenzaTimer = 0;

unsigned long tBlendOn = 0;
unsigned long tBlendOff = 0;
bool fBlendOn = true;
bool fBlendOff = true;
bool sBlending = false;

void loop() {

  checkBlending();

  if (digitalRead(btnDisp)) {
    digitalWrite(ledBtn, HIGH);
    bottoneCounter++;
    if (bottoneCounter > 2000) {
      bottone = true;
    }
  } else {
    bottoneCounter = 0;
    bottone = false;
  }

  if (digitalRead(sensPresenza)) {
    presenzaCounter++;
    if (presenzaCounter > 5000) {
      presenza = true;
    }
  } else {
    presenzaCounter = 0;
    presenza = false;
  }


  if (bottone) {  //se premo pulsante

    spegni();

    dPlay = millis();
    sPlay = 0;
    sBlending = true;
    while (sPlay < 5) {

      evoluzione();

      ledBtnBlink();
      checkBlending();
    }

  } else {

    if (presenza) {

      ledBtnPulse();
      movimento();
      presenzaTimer = millis();

      if (fBlendOn) {
        sBlending = true;
      }

    }

    else {  //se non c'è nessuno

      ledBtnPulse();
      movimento();

      if ((millis() - presenzaTimer) > 10000) {
        spegni();
        sBlending = false;
      }
    }
  }
}

void checkBlending() {
  if (sBlending) {
    if (fBlendOn) {
      if ((millis() - tBlendOn) > 5000) {
        digitalWrite(blending_04, HIGH);
        tBlendOff = millis();
        fBlendOn = false;
        fBlendOff = true;
        sBlending = false;
      }
    }
  } else {
    if (fBlendOff) {
      if ((millis() - tBlendOff) > 35000) {
        digitalWrite(blending_04, LOW);
        tBlendOn = millis();
        fBlendOff = false;
        fBlendOn = true;
      }
    }
  }
}

void spegni() {
  if ((millis() - tStall) > 60000) {
    tStall = millis();
    digitalWrite(airPump_01, HIGH);
    digitalWrite(ventoRX_02, HIGH);
  }

  if ((millis() - tStall) > 3000) {
    digitalWrite(airPump_01, LOW);
  }

  if ((millis() - tStall) > 20000) {
    if (millis() > 60000 * 60) {
      digitalWrite(ventoRX_02, LOW);
    }
  }


  digitalWrite(sideLED, LOW);

  digitalWrite(oilP_A_03, LOW);

  digitalWrite(airPump_05, LOW);
  digitalWrite(ledBtn, LOW);
}

unsigned long tMacerazione = 0, tPulse = 0, tVulcano = 0, tFilter = 0;
bool sMacerazione = false, sPulse = false, sVulcano = false;
int dFilter = 2000;

void ledBtnBlink() {
  if ((millis() - tPulse) > 250) {
    tPulse = millis();
    sPulse = !sPulse;
    digitalWrite(ledBtn, sPulse);
  }
}

void ledBtnPulse() {
  if ((millis() - tPulse) > 4) {
    tPulse = millis();

    if (i < 255) {
      i++;
    } else {
      i = 0;
    }
    analogWrite(ledBtn, i);
  }
}

void evoluzione() {

  //bollitura, macerazione e blending
  if (sPlay == 0) {
    //accendi la bollitura
    digitalWrite(airPump_01, HIGH);

    if ((millis() - dPlay) > 1000) {
      sPlay = 1;
      dPlay = millis();
    }
  }

  //vulcano e led
  else if (sPlay == 1) {
    //accendi led side
    digitalWrite(sideLED, HIGH);

    //accendi fontana in mezzo
    if ((millis() - dPlay) > 250) {
      digitalWrite(oilP_A_03, LOW);
    } else {
      digitalWrite(oilP_A_03, HIGH);
    }

    //manda segnale led
    digitalWrite(ledSignal, HIGH);

    if ((millis() - dPlay) > 500) {
      digitalWrite(ledSignal, LOW);
      sPlay = 2;
      dPlay = millis();
    }

  }

  //aspetta 4 secondi
  else if (sPlay == 2) {
    if ((millis() - dPlay) > 4000) {
      sPlay = 3;
      dPlay = millis();
    }
  }

  //vulcano lato e filtrazione
  else if (sPlay == 3) {

    digitalWrite(airPump_05, HIGH);

    if ((millis() - dPlay) > 1000) {
      sPlay = 4;
      dPlay = millis();
    }
  }

  //erogazione attesa finale
  else if (sPlay == 4) {
    if (!digitalRead(sensBottle)) {
      digitalWrite(dispencer_07, HIGH);
    }

    if ((millis() - dPlay) > 5000) {
      digitalWrite(dispencer_07, LOW);
      sPlay = 5;
    }
  }
}

void movimento() {
  //sempre accese / spente
  digitalWrite(airPump_01, HIGH);
  digitalWrite(sideLED, HIGH);
  digitalWrite(ventoRX_02, HIGH);

  //vulcano a scatti
  if (!sVulcano) {
    if ((millis() - tVulcano) > 6500) {
      tVulcano = millis();
      sVulcano = true;
      digitalWrite(oilP_A_03, HIGH);
    }
  } else {
    if ((millis() - tVulcano) > 250) {
      tVulcano = millis();
      sVulcano = false;
      digitalWrite(oilP_A_03, LOW);
    }
  }


  //filtrazione a scatti
  if ((millis() - tFilter) > dFilter) {
    tFilter = millis();
    if (dFilter == 2000) {
      digitalWrite(airPump_05, HIGH);
      dFilter = 250;
    } else {
      digitalWrite(airPump_05, LOW);
      dFilter = 2000;
    }
  }
}
