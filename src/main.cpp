#include <Arduino.h>
// Libraries
#include <RTClib.h> // Real Time Clock module
#include <EEPROM.h> // EEPROM partition from flash memory
#include <Wire.h> // I2C

// User Libraries
#include "pt_connection.h"
#include "pt_constants.h"
#include "pt_dispense.h"

static bool power_button_event = false;
static bool enable = false;
static short int state = 1;

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  if (power_button_event) {
    enable = !enable;
  }

  if (enable) {
    switch (state) {
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      default:
        state = 1;
        break;
    }
  }
}