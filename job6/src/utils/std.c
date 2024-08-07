#include "std.h"

void __fatal(char *file, int line_no, char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    fprintf(stderr, "fatal: ");
    vfprintf(stderr, format, ap);
    fprintf(stderr, "\n");

    va_end(ap);
    exit(EXIT_FAILURE);
}
