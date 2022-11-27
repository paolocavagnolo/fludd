#include <FastLED.h>

#define NUM_LEDS 64
#define COM_PIN 6
#define BTN_IN 7

CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812B, COM_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);

  FastLED.clear();
  FastLED.show();

  pinMode(BTN_IN, INPUT);
  Serial.begin(9600);

}

uint8_t j = 0;
uint8_t je = 32;
uint8_t i = 0;
uint8_t w1 = 0;
uint8_t w2 = 0;
uint8_t w3 = 0;
uint8_t w = 0;

bool beam = false;

unsigned long startT = 0;

void loop() {

  if (!digitalRead(BTN_IN)) {
    beam = true;
    startT = millis();
  }

  if (beam) {
    if ((millis() - startT) > 7200) {
      w1 = random(245, 255);
      w2 = random(0, 2);
      w3 = random(0, 1);

      if (w3) {
        w = w1;
      }
      else {
        w = w2;
      }

      for (int kk = 0; kk < 3; kk++) {
        for (i = 0; i < 254; i = i + 2) {
          fill_solid(leds, NUM_LEDS, CHSV(w, 255, i));
          FastLED.show();
          delay(10);
        }
        for (i = 254; i > 0; i = i - 2) {
          fill_solid(leds, NUM_LEDS, CHSV(w, 255, i));
          FastLED.show();
          delay(10);
        }
      }
     
      beam = false;
    }
    else {
      for (i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB::Red;
        FastLED.show();
        delay(5);
        leds[i] = CRGB::Black;
        FastLED.show();
        delay(5);
      }
    }
  }
  else {

    for (i = 0; i < 20; i++) {
      leds[j] = CHSV(224, 255, i);
      leds[je] = CHSV(224, 255, 19 - i);
      FastLED.show();
    }
    delay(30);
    j = j + 1;
    je = je + 1;

    if (j >= NUM_LEDS) {
      j = 0;
    }
    if (je >= NUM_LEDS) {
      je = 0;
    }
  }
}
