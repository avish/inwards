#include <TlcLed.h>


void setup() {
  TlcLed.init();
}

void loop() {
  TlcLed.clear();
  for (int strip = 0; strip < 10; strip++)
    TlcLed.setRgb(strip, 255, 0, 0);
  TlcLed.update();
  delay(250);

  TlcLed.clear();
  for (int strip = 0; strip < 10; strip++)
    TlcLed.setRgb(strip, 0, 255, 0);
  TlcLed.update();
  delay(250);

  TlcLed.clear();
  for (int strip = 0; strip < 10; strip++)
    TlcLed.setRgb(strip, 0, 0, 255);
  TlcLed.update();
  delay(250);

  TlcLed.clear();
  for (int strip = 0; strip < 10; strip++)
    TlcLed.setRgb(strip, 255, 255, 255);
  TlcLed.update();
  delay(250);

  TlcLed.clear();
  TlcLed.update();
  delay(250);   
}

