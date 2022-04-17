#include <Arduino.h>

struct PettBasicData{
        unsigned int TypeOfConfig; // If 1 -> easy configuration mode, if 2 -> customized configuration mode
        unsigned int RealTimeApp[6]; // DateTime real time received from App. Values -> {year, month, day, hour, minute, second}
        unsigned int PetBirthDay[6]; // DataTime pet's birthday received from App. Values -> {year, month, day, 0, 0, 0}
        unsigned int QuantityOfFood[3]; // Quantity of food received from (should be between 50 gr and 500 gr)
        unsigned int FoodRations[3]; // Food rations will divide the quantity of food daily (should be between 2 and 4)
};

struct PettDate {
        uint16_t year;
        uint8_t month;
        uint8_t day;
        uint8_t hour;
        uint8_t minute;
        uint8_t second;
};

struct PettDispenseData {
        uint16_t time;
        uint16_t amount;
};

#define BITS_0_TO_7(input)  (input & 0xFF)
#define BITS_8_TO_15(input) ((input & 0xFF00) >> 8)

#define DISPENSED_PACK_SIZE 4

enum EEPROM_ADDRESSES {
        STATE_ADDRESS,
        LAST_MODIFIED_WIFI_ADDRESS = 2,
        WIFI_SSID_ADDRESS = 9,
        WIFI_PASSWORD_ADDRESS = 109,
        BIRTH_DATE_ADDRESS = 209,
        LAST_MODIFICATION_OF_DISPENSED_ADDRESS = 213,
        NUMBER_OF_DISPENSED_ADDRESS = 220,
        DISPENSING_PACKAGES_ADDRESS = 222
};

enum EEPROM_STATES {
        DISCONFIGURED_EEPROM,
        WIFI_CONFIGURED,
        DISPENSED_CONFIGURED
};


// PINOUTS
#define SCREW_MOTOR 27 // Digital pin output D27 -> endless screw
#define LINEAR_MOTOR_R 25 // Digital pin output D34 -> linear actuator right turn (H bridge)
#define LINEAR_MOTOR_L 33 // Digital pin output D35 -> linear actuator left turn (H bridge)
#define DOUT_INT 19 // Data output pin 19 HX711 internal
#define CLK_INT 18 // Clock output pin 18 HX711 internal
#define DOUT_EXT 13 // Data output pin 13 HX711 external
#define CLK_EXT 14 // Clock output pin 14 HX711 external
// SCL -> 22 & SDA -> 21 || RTC

// CONSTANTS
#define TIME_SUP 20 // Hours in 24h format
#define TIME_INF 6 // Hours in 24h format
#define CONFIG_EASY 1 // Easy configuratioon preset for quick starting for user
#define CONFIG_CUSTOM 2 // Custom configuratioon preset for quick starting for user
#define MAX_DISPENSE_PER_DAY 10 // Maximum times dispensation will happen per day
#define TIMEOUT_DISPENSE 20 // Maximun time screw motor will be turning, if exceeded, dispense will stop (seconds)
#define LINEAR_ACTUATOR_TIME_DELAY 14 // Time linear actuator will move forewards and backwards in seconds

// MACROS
#define SETUP_EASY (EEPROM.read(0) == CONFIG_EASY) // Preset easy configuration -> 1
#define SETUP_CUSTOM (EEPROM.read(0) == CONFIG_CUSTOM) // Preset customized configuration -> 2
#define TIME_WINDOW (TIME_SUP - TIME_INF) // Window time in hours in 24h format