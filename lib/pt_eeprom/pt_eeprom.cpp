#include <Arduino.h>
#include <EEPROM.h>
#include <RTClib.h>

#include "pt_definitions.h"

extern struct PettBasicData pett_data;

extern RTC_DS3231 rtc_module;


void set_eeprom_state(uint8_t state)
{
        EEPROM.write(STATE_ADDRESS, state);
        EEPROM.write((STATE_ADDRESS + 1), ~state);

        EEPROM.commit();
}


/**
 *  @param eeprom_address First position of the memory space where the date is saved.
 */
void set_current_modification_date(uint16_t eeprom_address)
{
        PettDate last_modified_date = acquire_current_date();
        
        uint8_t lsb_year = BITS_0_TO_7(last_modified_date.year);
        uint8_t msb_year = BITS_8_TO_15(last_modified_date.year);

        EEPROM.write(eeprom_address, msb_year);
        EEPROM.write((eeprom_address + 1), lsb_year);
        EEPROM.write((eeprom_address + 2), last_modified_date.month);
        EEPROM.write((eeprom_address + 3), last_modified_date.day);
        EEPROM.write((eeprom_address + 4), last_modified_date.hour);
        EEPROM.write((eeprom_address + 5), last_modified_date.minute);
        EEPROM.write((eeprom_address + 6), last_modified_date.second);

        EEPROM.commit();
}


PettDate acquire_current_date()
{
        PettDate current_date;
        
        current_date.year = rtc_module.now().year();
        current_date.month = rtc_module.now().month();
        current_date.day = rtc_module.now().day();
        current_date.hour = rtc_module.now().hour();
        current_date.minute = rtc_module.now().minute();
        current_date.second = rtc_module.now().second();

        return current_date;
}


void set_date_of_birth(PettDate date_of_birth)
{
        uint8_t lsb_year = BITS_0_TO_7(date_of_birth.year);
        uint8_t msb_year = BITS_8_TO_15(date_of_birth.year);
        
        EEPROM.write(BIRTH_DATE_ADDRESS, msb_year);
        EEPROM.write((BIRTH_DATE_ADDRESS + 1), lsb_year);
        EEPROM.write((BIRTH_DATE_ADDRESS + 2), date_of_birth.month);
        EEPROM.write((BIRTH_DATE_ADDRESS + 3), date_of_birth.day);

        EEPROM.commit();
}


void set_number_of_dispensed(uint8_t number_of_dispensed)
{
        EEPROM.write(NUMBER_OF_DISPENSED_ADDRESS, number_of_dispensed);
        EEPROM.write((NUMBER_OF_DISPENSED_ADDRESS + 1), ~number_of_dispensed);

        EEPROM.commit();
}


/**
 *  @param package_number The packet number starting at 0.
 */
void set_dispensing_packs(uint8_t package_number, PettDispenseData dispense_data)
{
        EEPROM.write(
                (DISPENSING_PACKAGES_ADDRESS + (package_number * DISPENSED_PACK_SIZE)),
                BITS_8_TO_15(dispense_data.time)
        );
        EEPROM.write(
                ((DISPENSING_PACKAGES_ADDRESS + 1) + (package_number * DISPENSED_PACK_SIZE)),
                BITS_0_TO_7(dispense_data.time)
        );
        EEPROM.write(
                ((DISPENSING_PACKAGES_ADDRESS + 2) + (package_number * DISPENSED_PACK_SIZE)),
                BITS_8_TO_15(dispense_data.amount)
        );
        EEPROM.write(
                ((DISPENSING_PACKAGES_ADDRESS + 3) + (package_number * DISPENSED_PACK_SIZE)),
                BITS_0_TO_7(dispense_data.amount)
        );

        EEPROM.commit();
}


void set_dispensed_checksum(uint8_t number_of_dispensed)
{
        if (number_of_dispensed == 0) {
                EEPROM.write(DISPENSING_PACKAGES_ADDRESS, 0);
        } else {
                uint32_t checksum = 0;
                uint8_t last_dispensed_byte;

                last_dispensed_byte = (
                        (DISPENSING_PACKAGES_ADDRESS + 3)
                        + (DISPENSED_PACK_SIZE * number_of_dispensed)
                );


                for (uint8_t i = DISPENSING_PACKAGES_ADDRESS; i <= last_dispensed_byte; i++) {
                        checksum += EEPROM.read(i);
                }

                EEPROM.write(
                        (last_dispensed_byte + 1),
                        (uint8_t)(checksum & 0xFF)
                );
        }

        EEPROM.commit();
}