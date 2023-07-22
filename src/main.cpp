#include <Arduino.h>
#include "hardware/Display.h"

Display display;

void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! Feather TFT Test"));

  display.configure();
}

void loop() {

}

