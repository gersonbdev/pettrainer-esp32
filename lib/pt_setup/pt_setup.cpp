#include <RTClib.h>

RTC_DS3231 MyRtc;

/*
 * Initializes RTC configuration by using user's App information
 */
void initRTC()
{
        MyRtc.begin();
        delay(10);
        //MyRtc.adjust( DateTime(pett_data.RealTimeApp[0], pett_data.RealTimeApp[1], pett_data.RealTimeApp[2], pett_data.RealTimeApp[3], pett_data.RealTimeApp[4], pett_data.RealTimeApp[5]) ); // Sets real time from user's app
        //MyRtc.adjust(DateTime(2021, 11, 6, 11, 35, 49)); // TESTS
        MyRtc.adjust( DateTime(__DATE__, __TIME__) ); // Sets real time with from our PC
}