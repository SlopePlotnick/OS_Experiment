#include "std.h"

void __trace(char *file, int line_no, char *format, ...)
{
    va_list ap;
    char msg[256];

    va_start(ap, format);
    vsprintf(msg, format, ap);
    va_end(ap);

    fprintf(stderr, "%s: %d: ", file, line_no);
    fprintf(stderr, "%s\n", msg);
}

void __panic(char *file, int line_no, char *format, ...)
{
    va_list ap;
    char msg[256];

    va_start(ap, format);
    vsprintf(msg, format, ap);
    va_end(ap);

    fprintf(stderr, "%s: %d: Panic: ", file, line_no);
    if (errno == 0)
        fprintf(stderr, "%s\n", msg);
    else
        perror(msg);
    exit(EXIT_FAILURE);
}

void __allege(char *file, int line_no, int expr)
{
    if (!expr) {
        fprintf(stderr, "%s: %d: Allege failed\n", file, line_no);
        exit(EXIT_FAILURE);
    }
}
