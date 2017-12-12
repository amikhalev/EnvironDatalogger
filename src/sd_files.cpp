#include "sd_files.hpp"

#include <RTClib.h>
#include <stdint.h>
#include "printf.hpp"

const uint8_t days_in_month[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

uint16_t get_day_of_year(uint8_t month, uint8_t day)
{
    uint16_t day_of_year = day;
    uint8_t month_days;
    for (month = month - 1; month > 0; month--)
    {
        month_days = pgm_read_byte_near(&days_in_month[month - 1]);
        day_of_year += month_days;
    }
    return day_of_year;
}

SdFiles SdFiles::instance = SdFiles();

void SdFiles::timestamp_callback(uint16_t *date, uint16_t *time)
{
    const DateTime *rtc_now = SdFiles::instance.rtc_now;
    *date = FAT_DATE(rtc_now->year(), rtc_now->month(), rtc_now->day());
    *time = FAT_TIME(rtc_now->hour(), rtc_now->minute(), rtc_now->second());
}

int8_t SdFiles::begin(const DateTime *rtc_now)
{
    int8_t res;
    char new_filename[13]; // 8.3 + null
    uint8_t file_hour;
    uint16_t file_day_of_year;
    uint8_t file_number = 0;
    bool file_exists;

    this->rtc_now = rtc_now;
    SdFile::dateTimeCallback(SdFiles::timestamp_callback);

    info(F("Opening SD card..."));
    res = sd.begin(SD_CS_PIN, SD_SPI_SETTINGS);
    if (!res)
    {
        sd.initErrorPrint();
        return res;
    }
    else
    {
        res = sd.mkdir(LOGFILE_DIRECTORY);
#if DEBUG
        if (!res)
        {
            debugln(F("sd.mkidir(" LOGFILE_DIRECTORY ") fail"));
        }
#endif
        res = sd.chdir(LOGFILE_DIRECTORY, true);
        if (!res)
        {
            debugln(F("sd.chdir(" LOGFILE_DIRECTORY ") fail"));
            return res;
        }
        file_hour = rtc_now->hour();
        file_day_of_year = get_day_of_year(rtc_now->month(), rtc_now->day());
        while (true)
        {
            sprintf_P(new_filename, (const char *)LOGFILE_FORMAT, file_day_of_year, file_hour, file_number);
            file_exists = sd.exists(new_filename);
            debugf("sd.exists(%s)=%u\n", new_filename, file_exists);
            if (file_exists)
            {
                file_number++;
                continue;
            }
            break;
        }
        res = sd_file.open(&sd, new_filename, (O_RDWR | O_CREAT | O_AT_END));
        if (!res)
        {
            infof("sd_file.open(%s) fail\n", new_filename);
            return res;
        }
        infof("good. file=%s\n", new_filename);
        return true;
    }
}
