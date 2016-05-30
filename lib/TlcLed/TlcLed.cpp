#include <Arduino.h>
#include <Tlc5940.h>
#include "TlcLed.h"

#define MIN_INTENSITY 4095
#define MAX_INTENSITY_DEFAULT 3072

// Mono strip# ==> TLC channel#
const TLC_CHANNEL_TYPE STRIP_CHANNEL_LOOKUP[] = {
   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, // TLC 1, channel 15 unused
  17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31  // TLC 2, channel 16 unused
};

// RGB strip# => initial Mono strip#
const strip_t RGB_STRIP_LOOKUP[] = {
  0, 3, 6, 9, 12,
  15, 18, 21, 24, 27
};

uint16_t CTlcLed::intensityValue(val_t intensity) {
  return map(intensity, 0, 255, MIN_INTENSITY, _max_intensity_value);
}

val_t CTlcLed::valueIntensity(uint16_t value) {
  return map(value, MIN_INTENSITY, _max_intensity_value, 0, 255);
}

void CTlcLed::init() {
  init(MAX_INTENSITY_DEFAULT);
}

void CTlcLed::init(uint16_t max_intensity_value) {
  _max_intensity_value = max_intensity_value;
  Tlc.init(MIN_INTENSITY);
}


void CTlcLed::clear() {
  Tlc.setAll(MIN_INTENSITY);
}

void CTlcLed::setMono(strip_t strip, val_t intensity) {
  TLC_CHANNEL_TYPE tlc_channel = STRIP_CHANNEL_LOOKUP[strip];
  Tlc.set(tlc_channel, intensityValue(intensity));
}

val_t CTlcLed::getMono(strip_t strip) {
  TLC_CHANNEL_TYPE tlc_channel = STRIP_CHANNEL_LOOKUP[strip];
  return valueIntensity(Tlc.get(tlc_channel));
}

void CTlcLed::setAllMono(val_t intensity) {
  Tlc.setAll(intensityValue(intensity));
}

void CTlcLed::setRgb(strip_t strip, val_t r, val_t g, val_t b) {
  strip_t mono_strip = RGB_STRIP_LOOKUP[strip];
  setMono(mono_strip + 1, r);
  setMono(mono_strip + 2, g);
  setMono(mono_strip, b);
}

void CTlcLed::setRgb(strip_t strip, rgb_t rgb) {
  setRgb(strip, rgb.r, rgb.g, rgb.b);
}

rgb_t CTlcLed::getRgb(strip_t strip) {
  strip_t mono_strip = RGB_STRIP_LOOKUP[strip];
  return rgb_t(
    getMono(mono_strip + 1),
    getMono(mono_strip + 2),
    getMono(mono_strip));
}

void CTlcLed::setAllRgb(val_t r, val_t g, val_t b) {
  for (strip_t strip = 0; strip < NUM_RGB_STRIPS; strip++) {
    setRgb(strip, r, g, b);
  }
}

void CTlcLed::setAllRgb(rgb_t rgb) {
  setAllRgb(rgb.r, rgb.g, rgb.b);
}

uint8_t CTlcLed::update() {
  Tlc.update();
}

CTlcLed TlcLed;

#ifdef __INC_FASTSPI_LED2_H
  ColorCycler::ColorCycler(int stops, CRGB* colors, long* durations) {
    _stops = stops;
    _colors = colors;
    _durations = durations;
    _index = 0;
    _prevColor = CRGB(0);
  }

  void ColorCycler::start() {
    start(0);
  }

  void ColorCycler::start(long delay) {
    _start = millis() + delay;
  }

  CRGB ColorCycler::current() {
    CRGB targetColor = _colors[_index];
    long duration = _durations[_index];
    long elapsed = millis() - _start;

    if (elapsed < 0)
      return CRGB::Black;

    while (elapsed > duration) {
      _start += duration;
      elapsed -= duration;

      _index = (_index + 1) % _stops;
      _prevColor = targetColor;
      targetColor = _colors[_index];
      duration = _durations[_index];
    }

    return _prevColor.lerp16(targetColor, map(elapsed, 0, duration, 0, 65535));
  }

#endif
