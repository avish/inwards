#include <TlcLed.h>

void setup() {
  TlcLed.init();
}

void loop() {
  // Using RGB strips
  TlcLed.setAllRgb(255, 127, 64);
  TlcLed.update();
  delay(500);

  TlcLed.clear();
  TlcLed.update();
  delay(500);

  CRGB startColor = CRGB::Red;
  CRGB endColor = CRGB::Green;

  for (int strip = 0; strip < NUM_RGB_STRIPS; strip++) {
    // Lerp between start and end colors according to strip number (using FastLED's lerp8 function)
    CRGB stripColor = startColor.lerp8(endColor, map(strip, 0, NUM_RGB_STRIPS - 1, 0, 255));
    TlcLed.setRgb(strip, stripColor);
  }

  TlcLed.update();
  delay(500);

  TlcLed.clear();
  TlcLed.update();
  delay(500);

  // Using monochrome strips
  TlcLed.clear();
  for (int strip = 0; strip < NUM_MONO_STRIPS; strip++) {
    TlcLed.setMono(strip, 255);
    TlcLed.update();
    delay(200);
    TlcLed.setMono(strip, 0);
  }
}
