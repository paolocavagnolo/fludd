#include <FastLED.h>

#define NUM_LEDS 240
#define COM_PIN 9

#define SALITA 70
#define ORIZZ 180


CRGB leds[NUM_LEDS];

void setup() {

  pinMode(8,INPUT_PULLUP);
  
  FastLED.addLeds<WS2812B, COM_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  
  FastLED.clear();
  

  fill_solid(leds,NUM_LEDS,CHSV(25,255,150));
  FastLED.show();
  

}

uint8_t j = 0;
uint8_t i = 0;
uint8_t w = 0;

bool beam = false;

unsigned long startT = 0;
unsigned long fadeT = 0;

bool sali=true;

void loop() {

  if (!digitalRead(8)) {
    beam = true;
    startT = millis();
  }

  if (beam) {
    if ((millis() - startT) > 3) {
      for (i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(1,255,255);
        delay(25);
        FastLED.show();
      }
      beam = false;
    }
  }
  else {

    fill_solid(leds,NUM_LEDS,CHSV(25,255,j));
    FastLED.show();

    if ((millis() - fadeT) > 30) {
      fadeT = millis();
      if (sali) {
        j++;
      }
      else {
        j--;
      }
      if (j>150 || j<1) {
        sali = !sali;
      }
    } 
  }
}
