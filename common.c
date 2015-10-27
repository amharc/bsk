#include "common.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Noreturn void fail(bool show_errno, const char *format, ...) {
    va_list fmt_args;
    int err = errno;

    fprintf(stderr, "A fatal error has occurred. Giving up. Message: ");
    va_start(fmt_args, format);
    vfprintf(stderr, format, fmt_args);
    va_end(fmt_args);
    fprintf(stderr, "\n");

    if(err != 0 && show_errno)
        fprintf(stderr, "Error code: %d (%s)\n", err, strerror(err));

    exit(EXIT_FAILURE);
}
