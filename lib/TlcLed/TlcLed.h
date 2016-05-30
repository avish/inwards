#ifndef __TLCLED_H
#define __TLCLED_H

#include <Tlc5940.h>
#include <FastLED.h>

typedef uint8_t strip_t;
typedef uint8_t val_t;


struct rgb_t {
  val_t r;
  val_t g;
  val_t b;

  inline rgb_t(val_t ir, val_t ig, val_t ib) __attribute__((always_inline))
    : r(ir), g(ig), b(ib)
  {
  }

  inline rgb_t(uint32_t colorcode)  __attribute__((always_inline))
  : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >> 0) & 0xFF)
  {
  }

  inline rgb_t(CRGB& crgb)  __attribute__((always_inline))
  : r(crgb.r), g(crgb.g), b(crgb.b)
  {
  }

  inline rgb_t& operator=(CRGB crgb) __attribute__((always_inline))
  {
    r = crgb.r;
    g = crgb.g;
    b = crgb.b;
  }
};

#define NUM_MONO_STRIPS 30
#define NUM_RGB_STRIPS 10

class CTlcLed {
  private:
    uint16_t intensityValue(val_t intensity);
    val_t valueIntensity(uint16_t value);

  public:
    void init();
    void clear();

    void setAllMono(val_t val);
    void setMono(strip_t strip, val_t val);
    val_t getMono(strip_t strip);

    void setAllRgb(val_t r, val_t g, val_t b);
    void setAllRgb(rgb_t rgb);
    void setRgb(strip_t strip, val_t r, val_t g, val_t b);
    void setRgb(strip_t strip, rgb_t rgb);
    rgb_t getRgb(strip_t strip);

    uint8_t update();
};

extern CTlcLed TlcLed;

class ColorCycler {
  private:
    int _stops;
    CRGB* _colors;
    long* _durations;

    int _index;
    long _start;
    CRGB _prevColor;

  public:
    ColorCycler(int stops, CRGB* colors, long* durations);
    void start();
    void start(long delay);
    CRGB current();
};

#endif
