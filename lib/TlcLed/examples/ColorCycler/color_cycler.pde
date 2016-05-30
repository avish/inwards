#include <FastLED.h>
#include <TlcLed.h>

#define NUM_COLORS1 16
CRGB COLORS1[] = {
  0xFF0000, 0x00FF00, 0x0000FF, 0x808080,
  0xFFFF00, 0xFF00FF, 0x0000FF,
  0xFF8000, 0xFF0080, 0x80FF00, 0x8000FF,
  0x00FF80, 0x0080FF, 0x808080, 0x000000, 0x000000
};

long DURATIONS1[] = {
  1000, 2000, 1000, 2000,
  4000, 4000, 4000,
  500, 500, 500, 500,
  500, 500, 500, 500, 1000
};

#define NUM_COLORS2 4
CRGB COLORS2[] = { CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Black };
long DURATIONS2[] = { 1000, 1000, 1000, 1000 };

#define RESOLUTION 50


ColorCycler* cycler1;
ColorCycler* cycler2;

void setup() {
  TlcLed.init();
  TlcLed.clear();

  cycler1 = new ColorCycler(NUM_COLORS1, COLORS1, DURATIONS1);
  cycler2 = new ColorCycler(NUM_COLORS2, COLORS2, DURATIONS2);
}

void loop() {
  CRGB current1 = cycler1->current();
  for (int strip = 0; strip < NUM_RGB_STRIPS; strip += 2) {
    TlcLed.setRgb(strip, current1);
  }

  CRGB current2 = cycler2->current();
  for (int strip = 1; strip < NUM_RGB_STRIPS; strip += 2) {
    TlcLed.setRgb(strip, current2);
  }

  TlcLed.update();
  delay(RESOLUTION);
}
