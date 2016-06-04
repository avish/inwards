// LANDING WAVES
// for Backstage Group's INWARDS
// by Avish

#include <FastLED.h>

#define BRIGHTNESS_PULSE  // Comment this line to disable brightness pulse
#define HUE_GRADIENTS     // Comment this line to disable gradient hue shifts

#define LOGICAL_LEDS_DENSITY 20   // 20 Logical LEDs per meter = 60 physical LEDs (addressable in groups of 3)
#define GLOBAL_BRIGHTNESS 64      // Global brightness level, 0..255
#define BRIGHTNESS_PULSE_BPM 11   // BPM of brightness pulse

// Color Palette
// Due to my laziness, colors cannot cycle across the hue wheel's origin (i.e. you can't cross HUE 0 = Red)
uint8_t HUES[] = { HUE_GREEN - 16, HUE_GREEN + 16, HUE_AQUA };

// Data Pins of the 3 groups
#define G1_PIN 9
#define G2_PIN 10
#define G3_PIN 11

// Number of LEDs in each group
static const int G1_NUM_LEDS =    (LOGICAL_LEDS_DENSITY * 92 / 10);                 // 9.2m
static const int G2L1_NUM_LEDS =  (LOGICAL_LEDS_DENSITY * 29 / 10);                 // 2.9m
static const int G2L2_NUM_LEDS =  (LOGICAL_LEDS_DENSITY * 17 / 10);                 // 1.7m
static const int G2_NUM_LEDS =    (G2L1_NUM_LEDS + G2L2_NUM_LEDS + G2L2_NUM_LEDS);  // 2.9m + 2 x 1.7m
static const int G3_NUM_LEDS =    (LOGICAL_LEDS_DENSITY * 75 / 10);                 // 7.5m

CRGB G1_LEDS[G1_NUM_LEDS];
CRGB G2_LEDS[G2_NUM_LEDS];
CRGB G3_LEDS[G3_NUM_LEDS];

#define G2L1_LEDS G2_LEDS
#define G2L2_LEDS (&(G2_LEDS[G2L1_NUM_LEDS]))

class WaveSpace {
  private:
    CRGB* _leds;
    int _numLeds;
    int _waveSize;
    int _waveBeat;
    bool _reverse;

    uint8_t _startHue;
    #ifdef HUE_GRADIENTS
    uint8_t _endHue;
    saccum87 _deltaHue;
    #endif

   public:
    WaveSpace(CRGB* leds, int numLeds, bool reverse = false);
    void init(uint8_t startHue, uint8_t endHue, int waveSize, int waveBeat);
    void update();
};

WaveSpace::WaveSpace(CRGB* leds, int numLeds, bool reverse) {
  _leds = leds;
  _numLeds = numLeds;
  _reverse = reverse;
}

WaveSpace group1(     G1_LEDS,    G1_NUM_LEDS);
WaveSpace group2leg1( G2L1_LEDS,  G2L1_NUM_LEDS, true);
WaveSpace group2leg2( G2L2_LEDS,  G2L2_NUM_LEDS, true);
WaveSpace group3(     G3_LEDS,    G3_NUM_LEDS);

void WaveSpace::init(uint8_t startHue, uint8_t endHue, int waveSize, int waveBeat) {
  _startHue = startHue;

  #ifdef HUE_GRADIENTS
  _endHue = endHue;
  _deltaHue = (((int)_endHue - _startHue) << 8) / _numLeds;
  #endif

  _waveSize = waveSize;
  _waveBeat = waveBeat;
}

void WaveSpace::update() {
  uint8_t offset = beat8(_waveBeat);
  if (_reverse)
    offset = -offset;

  #ifdef HUE_GRADIENTS
  accum88 hue88 = _startHue << 8;
  #else
  uint8_t hue = _startHue;
  #endif

  for (int i = 0; i < _numLeds; i++) {
    uint8_t wavePos = map(i % _waveSize, 0, _waveSize, 0, 256); 
    uint8_t intensity = cubicwave8(wavePos - offset);

    #ifdef HUE_GRADIENTS
    uint8_t hue = hue88 >> 8;
    hue88 += _deltaHue;
    #endif
    
    _leds[i] = CHSV(hue, 255, intensity);
  }
}

void setup() {
  FastLED.addLeds<WS2811, G1_PIN, BRG>(G1_LEDS, G1_NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2811, G2_PIN, BRG>(G2_LEDS, G2_NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.addLeds<WS2811, G3_PIN, BRG>(G3_LEDS, G3_NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  
  fill_solid(G1_LEDS, G1_NUM_LEDS, CRGB::Black);
  fill_solid(G2_LEDS, G2_NUM_LEDS, CRGB::Black);
  fill_solid(G3_LEDS, G3_NUM_LEDS, CRGB::Black);

  // WaveSpace    Start Hue End Hue   Wave Size   Wave BPM
  group2leg2.init(HUES[0],  HUES[1],  20,         30);
  group2leg1.init(HUES[1],  HUES[2],  30,         45);
  group1.init(    HUES[2],  HUES[1],  40,         60);
  group3.init(    HUES[1],  HUES[0],  40,         75);
}

void loop() {
  group1.update();
  group2leg1.update();
  group2leg2.update();
  group3.update();

  // Mirror G2L2 in reverse to create parallel walls
  for (int i = 0; i < G2L2_NUM_LEDS; i++) {
    int source_index = G2L1_NUM_LEDS + i;
    int destination_index = G2_NUM_LEDS - 1 - i;
    G2_LEDS[destination_index] = G2_LEDS[source_index];
  }

  #ifdef BRIGHTNESS_PULSE
  // Pulse overall brightness
  FastLED.setBrightness(beatsin8(BRIGHTNESS_PULSE_BPM, GLOBAL_BRIGHTNESS >> 3, GLOBAL_BRIGHTNESS));
  #endif

  FastLED.show();
}

