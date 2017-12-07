#ifndef __DATETIME_UTIL_HPP__
#define __DATETIME_UTIL_HPP__

#include <Print.h>
#include <HardwareSerial.h>

class DateTime;

void print_datetime_date(DateTime &datetime, Print &out = Serial);
void print_datetime_time(DateTime &datetime, Print &out = Serial);
void print_datetime(DateTime &datetime, Print &out = Serial);

#endif // __DATETIME_UTIL_HPP__