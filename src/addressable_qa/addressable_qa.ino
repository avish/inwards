#include "FastLED.h"

#define NUM_LEDS 100
#define MAX_POS (NUM_LEDS - 1)
#define DATA_PIN 6
#define DELAY 10

CRGB leds[NUM_LEDS];
int pos;
int dpos;

CRGB COLOR_ON = CRGB::White;
uint8_t COLOR_DECAY = 64;

void setup() { 
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); 
  pos = 0; 
  dpos = 1;
}

void loop() { 
  fadeAll(COLOR_DECAY);
  incrementPosition();
  
  leds[pos] = getPositionColor(pos);

  FastLED.show(); 
  delay(DELAY); 
}

void fadeAll(uint8_t decay) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i].nscale8(256 - decay);
  }
}

void incrementPosition() {
  pos += dpos;
  if (pos > MAX_POS) {
    dpos = -dpos;
    pos = MAX_POS - (pos - MAX_POS);
  } else if (pos < 0) {
    dpos = -dpos;
    pos = -pos;
  }
}

CRGB getPositionColor(int pos) {
  uint8_t pos8 = map(pos, 0, NUM_LEDS - 1, 0, 255);
  CRGB color = CHSV(pos8, 255, 255);
  return color;
}

