// TOWER FIREWORKS
// 1-dimensional Fireworks animation for Backstage Group's INWARDS
// By Avish

#include "FastLED.h"

#define DATA_PIN 6

#define NUM_LEDS 200            // 10m at 20 logical leds/m
#define FRAMERATE 60            // Frames per second, approx.

#define GLOBAL_BRIGHTNESS 255   // 8-bit global brightness value
#define MAX_SPARKS 100          // Max number of sparks; each explosion is 2 sparks
#define MIN_HUE 0
#define MAX_HUE 255
#define MIN_SAT 128
#define MAX_SAT 255
#define MIN_LUM 128
#define MAX_LUM 255
#define MIN_VEL 15      // Spark min velocity in LEDs per frame; adjust manually for framerate and led density
#define MAX_VEL 30      // Spark max velocity in LEDs per frame; adjust manually for framerate and led density
#define VEL_DECAY 230   // Velocity decay factor as an 8-bit fraction (e.g. 192 ==> velocity *= 3/4 each frame). Adjust manually for framerate

#define SPARK_CHANCE 10     // 8-bit chance of new spark per LED per second (256 / 20 = 12.8 = 1 spark per meter per second)
#define DECAY 1600          // Trail decay factor (framerate independent)
#define SPARK_DECAY 1200      // Spark decay factor (framerate independent)

CRGB leds[NUM_LEDS];

class Spark {
  private:
    accum88 _pos;
    saccum87 _vel;
    
  public:
    CRGB color;
    int pos();

    Spark();
    Spark(CRGB color, int pos, saccum87 vel);
    void update();

    bool dead;
};

Spark sparks[MAX_SPARKS];
int liveSparks = 0;

void setup() { 
  delay(3000); // Recovery?
  
  FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip); 
  FastLED.setBrightness(GLOBAL_BRIGHTNESS);
  
  fill_solid(leds, NUM_LEDS, CRGB::Black);
}

void createNewSparks() {
  if (liveSparks < MAX_SPARKS - 2 && random8() < SPARK_CHANCE * NUM_LEDS / FRAMERATE) {
    int sparkPos = random16(NUM_LEDS);
    CHSV color = CHSV(random8(MIN_HUE, MAX_HUE), random8(MIN_SAT, MAX_SAT), random8(MIN_LUM, MAX_LUM));
    saccum87 velocity = random16(MIN_VEL << 8, MAX_VEL << 8) / FRAMERATE;

    for (int i = 0; i < MAX_SPARKS; i+= 2) {
      if (sparks[i].dead && sparks[i+1].dead) {
        sparks[i] = Spark(color, sparkPos, velocity);
        sparks[i + 1] = Spark(color, sparkPos, -velocity);
        return;
      }
    }
    liveSparks += 2;
  }
}

void renderSparks() {
  for (int i = 0; i < MAX_SPARKS; i++) {
    Spark& spark = sparks[i];

    if (spark.dead)
      continue;
      
    leds[spark.pos()] += CRGB(spark.color);
    
    spark.update();
    if (spark.dead) {
      liveSparks--;
    }
  }
}

void loop() { 
  fadeToBlackBy(leds, NUM_LEDS, DECAY / FRAMERATE);

  createNewSparks();
  renderSparks();

  FastLED.show();
  FastLED.delay(1000 / FRAMERATE); 
}
  
Spark::Spark() {
  dead = true;
}

Spark::Spark(CRGB icolor, int ipos, saccum87 ivel) {
  color = icolor;
  _pos = ipos << 8;
  _vel = ivel;
  dead = false;
}

void Spark::update() {
  if (dead) {
    return;
  }
 
  _pos += _vel;
  if (_pos < 0 || _pos > NUM_LEDS << 8) {
    dead = true;
    return;
  }

  _vel = (_vel * VEL_DECAY) >> 8;

  color.fadeToBlackBy(SPARK_DECAY / FRAMERATE);
  if (!color) {
    dead = true;
  }
}

int Spark::pos() {
  return _pos >> 8;
}

