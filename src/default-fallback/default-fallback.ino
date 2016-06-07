// DEFAULT FALLBACK
// For Backstage Group's INWARDS
// By Avish

// If using this for default lighting, change the DURATIONS
// so they are longer. You can also add other colors or even
// use HSV ones

#include <FastLED.h>
#include <TlcLed.h>

#define NUM_COLORS 16
CRGB COLORS[] = { 
  0xFF0000, 0x00FF00, 0x0000FF, 0x808080,
  0xFFFF00, 0xFF00FF, 0x0000FF, 
  0xFF8000, 0xFF0080, 0x80FF00, 0x8000FF,
  0x00FF80, 0x0080FF, 0x808080, 0x000000, 0x000000
};

long DURATIONS[] = {
  1000, 2000, 1000, 2000, 
  4000, 4000, 4000,
  500, 500, 500, 500,
  500, 500, 500, 500, 1000
};

#define RESOLUTION 50

ColorCycler* cycler;

void setup() {
  TlcLed.init();
  TlcLed.clear(); 
  
  cycler = new ColorCycler(NUM_COLORS, COLORS, DURATIONS);
}

void loop() {
  CRGB current = cycler->current();
  TlcLed.setAllRgb(current);
  TlcLed.update();
  delay(RESOLUTION);
}
