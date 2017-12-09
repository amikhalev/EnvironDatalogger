#ifndef __SD_FILES_HPP__
#define __SD_FILES_HPP__

#include <SdFat.h>

class DateTime;

class SdFiles {
private:
    SdFat sd;
    SdFile sd_file;
    const DateTime* rtc_now;

    inline SdFiles() : sd(), sd_file(), rtc_now(NULL) { }

    static void timestamp_callback(uint16_t* date, uint16_t* time);

public:
    int8_t begin(const DateTime* rtc_now);

    inline bool isOpen() { return sd_file.isOpen(); }
    inline SdFile& file() { return sd_file; }
    inline bool sync() { return sd_file.sync(); }

    static SdFiles instance;
};

#endif // __SD_FILES_HPP__
