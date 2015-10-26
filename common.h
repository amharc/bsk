#ifndef _COMMON_H
#define _COMMON_H

_Noreturn void fail(const char *format, ...) __attribute__((format(printf, 1, 2)));

#endif /* !_COMMON_H */
