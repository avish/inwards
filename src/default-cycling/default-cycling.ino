// DEFAULT-CYCLING
// For Backstage Group's INWARDS
// By Avish

// When compiling, make sure tlc_config.h defines NUM_TLCS as 2

#include <FastLED.h>
#include <TlcLed.h>

#define RESOLUTION 10
#define TLC_MAX_INTENSITY 3072

#define MIN_COLOR_DURATION 10000
#define MAX_COLOR_DURATION 60000

#define MIN_SHIFT_DURATION 1000
#define MAX_SHIFT_DURATION 5000

#define MIN_HUE 0
#define MAX_HUE 255

#define MIN_SAT 192
#define MAX_SAT 255

#define MIN_LUM 32
#define MAX_LUM 64

class State {
  private:
    int _start;
    int _duration;

  protected:
    virtual void updateFrame(int elapsed, int duration) = 0;
    virtual State* nextState() = 0;

  public:
    State(int duration) {
      this->_duration = duration;
      this->_start = millis();
    }

    State* run() {
      int elapsed = millis() - this->_start;
      if (elapsed >= this->_duration) {
        State* next = this->nextState();
        delete this;
        return next;
      }
      else {
        this->updateFrame(elapsed, this->_duration);
        return this;
      }
    }
};

State* newShiftState(CHSV fromColor);
State* newColorState(CHSV color);

class ColorState : public State {
  private:
    CHSV _color;

  protected:
    void updateFrame(int elapsed, int duration) {
      CRGB rgbColor = this->_color;
      TlcLed.setAllRgb(rgbColor.r, rgbColor.g, rgbColor.b);
    }

    State* nextState() {
      State* next = newShiftState(_color);
      delete this;
      return next;
    }

  public:
    ColorState(int duration, CHSV color) : State(duration) {
      this->_color = color;
    }
};

class ShiftState : public State {
  private:
    CHSV _fromColor;
    CHSV _toColor;

  protected:
    void updateFrame(int elapsed, int duration) {
      uint8_t blendAmount = map(elapsed, 0, duration, 0, 256);
      CRGB color = blend(_fromColor, _toColor, blendAmount);
      TlcLed.setAllRgb(color.r, color.g, color.b);
    }

    State* nextState() {
      State* next = newColorState(_toColor);
      delete this;
      return next;
    }

  public:
    ShiftState(int duration, CHSV fromColor, CHSV toColor) : State(duration) {
      _fromColor = fromColor;
      _toColor = toColor;
    }
};

State* newColorState(CHSV color) {
  int duration = random(MIN_COLOR_DURATION, MAX_COLOR_DURATION);
  return new ColorState(duration, color);
}

State* newShiftState(CHSV fromColor) {
  CHSV nextColor = CHSV(random8(MIN_HUE, MAX_HUE), random8(MIN_SAT, MAX_SAT), random8(MIN_LUM, MAX_LUM));
  int duration = random(MIN_SHIFT_DURATION, MAX_SHIFT_DURATION);
  return new ShiftState(duration, fromColor, nextColor);
}

State* state = NULL;

void setup() {
  TlcLed.init(TLC_MAX_INTENSITY);
  TlcLed.clear();

  state = new ColorState(10000, CHSV(192, 255, 255));
}

void loop() {
  state = state->run();
  TlcLed.update();
  delay(RESOLUTION);
}
