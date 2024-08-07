#include "utils/std.h"
#include "log.h"

int log_on;

void log_printf(char *fmt, ...)
{
    if (log_on == 0)
        return;

    va_list ap;
    va_start(ap, fmt);

    write(1, "\033[32m", 5);
    vfprintf(stderr, fmt, ap);
    write(1, "\033[m", 3);

    va_end(ap);
}

