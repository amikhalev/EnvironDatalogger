#include "datetime_util.hpp"

#include <RTClib.h>

void print_datetime_date(DateTime &datetime, Print &out)
{
    out.print(datetime.year());
    out.print('-');
    if (datetime.month() < 10)
        out.print('0');
    out.print(datetime.month());
    out.print('-');
    if (datetime.day() < 10)
        out.print('0');
    out.print(datetime.day());
}

void print_datetime_time(DateTime &datetime, Print &out)
{
    if (datetime.hour() < 10)
        out.print('0');
    out.print(datetime.hour());
    out.print(':');
    if (datetime.minute() < 10)
        out.print('0');
    out.print(datetime.minute());
    out.print(':');
    if (datetime.second() < 10)
        out.print('0');
    out.print(datetime.second());
}

void print_datetime(DateTime &datetime, Print &out)
{
    print_datetime_date(datetime, out);
    out.print(' ');
    print_datetime_time(datetime, out);
}
