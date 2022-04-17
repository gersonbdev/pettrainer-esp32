#include <RTClib.h>

RTC_DS3231 rtc_module;

/*
 * Initializes RTC configuration by using user's App information
 */
void initRTC()
{
        rtc_module.begin();
        delay(10);
        //rtc_module.adjust( DateTime(pett_data.RealTimeApp[0], pett_data.RealTimeApp[1], pett_data.RealTimeApp[2], pett_data.RealTimeApp[3], pett_data.RealTimeApp[4], pett_data.RealTimeApp[5]) ); // Sets real time from user's app
        //rtc_module.adjust(DateTime(2021, 11, 6, 11, 35, 49)); // TESTS
        rtc_module.adjust( DateTime(__DATE__, __TIME__) ); // Sets real time with from our PC
}