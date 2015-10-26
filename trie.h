#ifndef _TRIE_H
#define _TRIE_H

#include <stddef.h>

/* An opaque type representing a TRIE */
struct trie;

/* Creates a new, empty TRIE */
struct trie* trie_create(void)
    __attribute__((returns_nonnull));

/* Frees the resources held by a TRIE */
void trie_free(struct trie *restrict)
    __attribute__((nonnull));

/* Inserts a word into a TRIE */
void trie_insert(struct trie *restrict tree, const char *restrict word, size_t length)
    __attribute__((nonnull(1, 2)));

/* Gets an arbitrary word that has been inserted even (but positive) number of times. 
 *
 * Returns NULL if no such word exists.
 */
const char* trie_get_even(struct trie *restrict)
    __attribute__((nonnull));

#endif /* !_TRIE_H */
