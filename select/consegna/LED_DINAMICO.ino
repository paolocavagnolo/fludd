//ARDUINO UNO

#include <FastLED.h>

#define NUM_LEDS 240
#define COM_PIN 5
#define BTN_IN 7

#define SALITA 70
#define ORIZZ 180


CRGB leds[NUM_LEDS];

void setup() {

  pinMode(BTN_IN, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, COM_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  FastLED.clear();


  fill_solid(leds, NUM_LEDS, CHSV(25, 255, 150));
  FastLED.show();

  Serial.begin(9600);


}

uint8_t j = 0;
uint8_t i = 0;
uint8_t w = 0;

bool beam = false;

unsigned long startT = 0;
unsigned long fadeT = 0;

bool sali = true, fadeOut = false;
unsigned long tShow = 0;
int idxBeam = 0;
unsigned long tBeam = 0, attesa = 25;

long counterBtnIn = 0;

void loop() {

  if (!beam) {
    if (!digitalRead(BTN_IN)) {
      counterBtnIn++;
    } else {
      counterBtnIn = 0;
    }

    if (counterBtnIn > 10000) {
      beam = true;
      fadeOut = false;
      attesa = 25;
      startT = millis();
      counterBtnIn = 0;
    }
  }
  
  if (beam) {

    if ((millis() - startT) > 3) {

      if ((millis() - tBeam) > attesa) {
        tBeam = millis();

        if (!fadeOut) {
          if (idxBeam < NUM_LEDS) {
            leds[idxBeam] = CHSV(map(idxBeam, 0, NUM_LEDS, 5, -6), 255, 255);
            idxBeam++;
          } else {
            attesa = 2500;
            fadeOut = true;
            idxBeam = 255;
          }
        } else {
          attesa = 8;
          for (i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(map(i, 0, NUM_LEDS, 5, -6), 255, idxBeam);
          }
          if (idxBeam > 0) {
            idxBeam--;
          } else {
            beam = false;
          } 
        }
      }
    }

  }

  else {

    if ((millis() - fadeT) > 10) {
      fadeT = millis();
      fill_solid(leds, NUM_LEDS, CHSV(-6, 255, j));
      if (sali) {
        j++;
      }
      else {
        j--;
      }
      if (j > 100 || j < 1) {
        sali = !sali;
      }
    }

  }


  if ((millis() - tShow) > (1000/25)) {
    tShow = millis();
    FastLED.show();
  }

}
