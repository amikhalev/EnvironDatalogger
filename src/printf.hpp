#ifndef __PRINTF_HPP__
#define __PRINTF_HPP__

#include <WString.h>
#include <Printable.h>
#include <stdarg.h>
#include <stdio.h>

#define PRINTF_MAX_LEN 100

class _Printf_Helper : public Printable
{
  private:
    char m_formatted[PRINTF_MAX_LEN];

  public:
    inline _Printf_Helper(const char *format, ...);
    inline _Printf_Helper(const __FlashStringHelper *format, ...);

    size_t printTo(Print &p) const override;
};

inline _Printf_Helper::_Printf_Helper(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsnprintf(m_formatted, PRINTF_MAX_LEN, format, args);
    va_end(args);
}

inline _Printf_Helper::_Printf_Helper(const __FlashStringHelper *format, ...)
{
    va_list args;
    va_start(args, format);
#ifdef __AVR__
    vsnprintf_P
#else
    vsnprintf
#endif
        (m_formatted, PRINTF_MAX_LEN, (const char *)format, args);
    va_end(args);
}

#define PRINTF(fmt, ...) _Printf_Helper(F(fmt), __VA_ARGS__)

#endif // __PRINTF_HPP__
