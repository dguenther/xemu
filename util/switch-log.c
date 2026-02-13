#include "qemu/osdep.h"

#ifndef CONFIG_SWITCH
void switch_log(const char *format, ...)
{
    va_list args;

    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}
#endif
