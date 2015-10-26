#ifndef _UNBOUNDED_STRING_H
#define _UNBOUNDED_STRING_H

#include <stddef.h>

/* An abstract type representing an unbounded, automatically growing string */
struct unbounded_string;

/* Converts a C-string to an unbounded_string. */
struct unbounded_string* us_from_string(const char *restrict)
    __attribute__((nonnull, returns_nonnull));

/* Releases resources held by an unbounded_string. */
void us_free(struct unbounded_string *restrict)
    __attribute__((nonnull));

/* Converts an unbounded_string to a C-string.
 *
 * Runs in constant time, returns a view as a C-string.
 */
const char* us_to_string(const struct unbounded_string *restrict)
    __attribute__((nonnull, returns_nonnull));

/* Pushes a character at the end of an unbounded_string. */
void us_push(struct unbounded_string *restrict, char)
    __attribute__((nonnull(1)));

/* Removes a character from the end of an unbounded_string. */
void us_pop(struct unbounded_string *restrict)
    __attribute__((nonnull));

/* Returns the length of an unbounded_string. */
size_t us_length(struct unbounded_string *restrict)
    __attribute__((nonnull, pure));

#endif /* !_UNBOUNDED_STRING_H */
