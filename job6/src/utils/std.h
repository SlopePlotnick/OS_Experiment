#ifndef _STD_H
#define _STD_H

#include <assert.h>
#include <ctype.h>
#include <limits.h>
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
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

extern void __fatal(char *file, int line_no, char *format, ...);
#define fatal(...) __fatal(__FILE__, __LINE__, __VA_ARGS__)

#endif
