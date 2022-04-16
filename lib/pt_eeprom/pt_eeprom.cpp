#include <Arduino.h>
#include <EEPROM.h>

#include "pt_definitions.h"

extern struct PettBasicData pett_data;


void set_modification_date(PettDate *last_modified_date)
{
        uint8_t msb_year = ((*last_modified_date).year & 0xFF00) >> 8;
        uint8_t lsb_year = ((*last_modified_date).year & 0xFF);

        EEPROM.write(1, msb_year);
        EEPROM.write(2, lsb_year);
        EEPROM.write(3, (*last_modified_date).month);
        EEPROM.write(4, (*last_modified_date).day);
        EEPROM.write(5, (*last_modified_date).hour);
        EEPROM.write(6, (*last_modified_date).minute);
        EEPROM.write(7, (*last_modified_date).second);

        EEPROM.write(8, ~msb_year);
        EEPROM.write(9, ~lsb_year);
        EEPROM.write(10, ~((*last_modified_date).month));
        EEPROM.write(11, ~((*last_modified_date).day));
        EEPROM.write(12, ~((*last_modified_date).hour));
        EEPROM.write(13, ~((*last_modified_date).minute));
        EEPROM.write(14, ~((*last_modified_date).second));

        EEPROM.commit();
}