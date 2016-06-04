// VOOMVOOM
// for Backstage Group's INWARDS
// by Avish

#include <TlcLed.h>

#define NUM_STRIPS 30     // Number of strips we control
#define TRAVEL_TIME 7000  // Time in millis for voomvoom wave to travel across all strips
#define CYCLE_TIME 5000   // Time in millis between single voom ending and starting anew (can be less than TRAVEL_TIME for multiple concurrent waves)
#define INTENSITY_DAMP  0 // Dampening of maximum intensity; default is 3072. A value of 0 Burns the board when left on too long.


ColorCycler* cyclers[NUM_STRIPS];

long CYCLE_DURATIONS[] = {
  50,   // Fade-in time
  0,   // On time
  500, // Fade-out time
  CYCLE_TIME // Wait time before starting again
};

CRGB CYCLE_COLORS[] = { 
  0xFFFFFF, // Fade-in
  0xFFFFFF, // On
  0x000000, // Fade-out
  0x000000  // Off
};

#define CYCLER_OFFSET (TRAVEL_TIME / NUM_STRIPS)
#define UPDATE_RESOLUTION 0     // Millis between updates

void setup() {
  TlcLed.init(INTENSITY_DAMP);
  
  for (int i = 0; i < NUM_STRIPS; i++) {
    cyclers[i] = new ColorCycler(4, CYCLE_COLORS, CYCLE_DURATIONS);
    cyclers[i]->start(CYCLER_OFFSET * i);
  }
}

void loop() {
  for (int i = 0; i < NUM_STRIPS; i++) {
    val_t current = cyclers[i]->current().red;
    TlcLed.setMono(i, current);
  }
  TlcLed.update();
  delay(UPDATE_RESOLUTION);
}

