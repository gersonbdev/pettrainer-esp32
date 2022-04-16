#include <Arduino.h>

// Libraries
#include <SPI.h>
#include <RTClib.h> // Real Time Clock module
#include <EEPROM.h> // EEPROM partition from flash memory
#include <Wire.h> // I2C

// User Libraries
#include "pt_constants.h"
#include "pt_setup.h"
#include "pt_connection.h"
#include "pt_dispense.h"

void setup() {
        Serial.begin(115200);

        xTaskCreate(
                task_dispense,
                "Task dispense",
                1000,
                NULL,
                1,
                NULL
        );

        xTaskCreate(
                task_connection,
                "Task connection",
                1000,
                NULL,
                1,
                NULL
        );
}

void loop() {
}