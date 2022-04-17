#ifndef pett_eeprom_h
#define pett_eeprom_h

#include "pett_definitions.h"

void set_eeprom_state(uint8_t state);

PettDate acquire_current_date();

void set_current_modification_date(uint16_t eeprom_address);

void set_date_of_birth(PettDate date_of_birth);

void set_number_of_dispensed(uint8_t number_of_dispensed);

void set_dispensing_packs(uint8_t package_number, PettDispenseData dispense_data);

void set_dispensed_checksum(uint8_t number_of_dispensed);

#endif