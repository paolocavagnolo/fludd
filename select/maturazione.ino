#include <FastLED.h>

#define NUM_LEDS 64 
#define COM_PIN 10

CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<WS2812B, COM_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(255);
  
  FastLED.clear();
  FastLED.show();

  pinMode(11,INPUT_PULLUP);
  Serial.begin(9600);

}

uint8_t j = 0;
uint8_t i = 0;
uint8_t w1 = 0;
uint8_t w2 = 0;
uint8_t w3 = 0;
uint8_t w = 0;

bool beam = false;

unsigned long startT = 0;

void loop() {

  if (!digitalRead(11)) {
    beam = true;
    startT = millis();
  }

  if (beam) {
    if ((millis() - startT) > 7000) {
      w1 = random(245,255);
      w2 = random(0,10);
      w3 = random(0,1);
      
      if (w3) {
        w = w1;
      }
      else {
        w = w2;
      }
      
      for (i = 0; i < 254; i = i + 2) {
        fill_solid(leds,NUM_LEDS,CHSV(w,255,i));
        FastLED.show();
        delay(5);
      }
      for (i = 254; i > 0; i = i - 2) {
        fill_solid(leds,NUM_LEDS,CHSV(w,255,i));
        FastLED.show();
        delay(1);
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
    
    for (i = 0; i < 254; i = i + 2) {
      leds[j] = CHSV(w,255,i);
      FastLED.show();
    }
  
    leds[j] = CHSV(w,255,255);
    FastLED.show();
  
    for (i = 0; i < NUM_LEDS; i++) {
      leds[i].nscale8(220);
      FastLED.show();
    }
  
    j = j + 1;
  
    if (j >= NUM_LEDS) {
      j = 0;
    }
  }
}
