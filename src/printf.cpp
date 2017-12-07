#include "printf.hpp"

#include <Print.h>

size_t _Printf_Helper::printTo(Print &out) const
{
    return out.print(m_formatted);
}
