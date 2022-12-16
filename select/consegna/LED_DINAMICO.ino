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

bool sali = true;

void loop() {

  Serial.println(digitalRead(BTN_IN));

  if (!digitalRead(BTN_IN)) {
    beam = true;
    startT = millis();
  }

  if (beam) {
    if ((millis() - startT) > 3) {
      for (i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(map(i, 0, NUM_LEDS, 5, -6), 255, 255);
        delay(25);
        FastLED.show();
      }
      delay(2500);
      for (int jj = 255; jj >= 0; jj--) {
        for (i = 0; i < NUM_LEDS; i++) {
          leds[i] = CHSV(map(i, 0, NUM_LEDS, 5, -6), 255, jj);
        }
        FastLED.show();
      }
      beam = false;
    }
  }
  else {

    fill_solid(leds, NUM_LEDS, CHSV(-6, 255, j));
    FastLED.show();

    if ((millis() - fadeT) > 10) {
      fadeT = millis();
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
}
