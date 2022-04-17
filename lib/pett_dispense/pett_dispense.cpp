#include <Arduino.h>
#include <RTClib.h>

#include "pett_definitions.h"

extern RTC_DS3231 rtc_module;
extern PettBasicData pett_data;

// Pet's age calculation control variables
unsigned int age_in_days; // Pet's age in days

// Dispensation TIME control variables
byte cont = 0; // counter of dispensation times throughout the day
int foodrations; // rations we will dispense throughout the day
int quantityoffood; // quantity of food (gr) we'd like to dispense per day
int DispenseTimes[MAX_DISPENSE_PER_DAY][2]; // times dispensation matrix


void task_dispense(void * pvParameters)
{
        int count = 0;
        while (1) {
                Serial.print("Task 1 counter:");
                Serial.print(count++);
                vTaskDelay(1000/portTICK_PERIOD_MS);
        }
}

/*
 * Calculates pet's age by using rtc_module instance
 */
void calculate_pet_age()
{
        // Calculation pet's age
        age_in_days = (
                (rtc_module.now().year()*365 + rtc_module.now().month()*30
                + rtc_module.now().day()+1) - (pett_data.PetBirthDay[0]*365
                + pett_data.PetBirthDay[1]*30 + pett_data.PetBirthDay[2])
        );

        // Prints total days and pet's age
        Serial.println("PET'S AGE");
        Serial.println("Total Days: " + String(age_in_days));
        Serial.println("");
}

void set_dispensing_based_on_age()
{
        if (age_in_days <= 90) {
                quantityoffood = pett_data.QuantityOfFood[0];
                foodrations = pett_data.FoodRations[0];
        } else if (age_in_days>90 && age_in_days<=180) {
                quantityoffood = pett_data.QuantityOfFood[1];
                foodrations = pett_data.FoodRations[1];
        } else if(age_in_days > 180) {
                quantityoffood = pett_data.QuantityOfFood[2];
                foodrations = pett_data.FoodRations[2];
        }
}