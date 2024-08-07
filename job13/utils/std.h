#ifndef _STD_H
#define _STD_H

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern void __trace(char *file, int line_no, char *format, ...);
#define trace(...) __trace(__FILE__, __LINE__, __VA_ARGS__)

extern void __panic(char *file, int line_no, char *format, ...);
#define panic(...) __panic(__FILE__, __LINE__, __VA_ARGS__)

extern void __allege(char *file, int line_no, int expr);
#define allege(expr) __allege(__FILE__, __LINE__, expr)

#define INLINE static __attribute__((always_inline)) inline

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

static inline unsigned long round_up(unsigned long size, int mod)
{
    return (size + mod - 1) & ~(mod - 1);  
}

static inline unsigned long round_down(unsigned long size, int mod)
{
    return size & ~(mod - 1);  
}

#endif
