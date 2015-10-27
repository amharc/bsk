#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

#define WITH_ERRNO true
#define WITHOUT_ERRNO false

_Noreturn void fail(bool show_errno, const char *format, ...) __attribute__((format(printf, 2, 3)));

#endif /* !_COMMON_H */
