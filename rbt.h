#ifndef _RBT_H
#define _RBT_H

#include "common.h"

#include <stdbool.h>

/* The type of keys in a red-black tree */
typedef char rb_key;

/* An opaque type representing a red-black tree */
struct rb_tree;

typedef void (*rb_callback)(struct rb_tree *restrict tree, rb_key key,
            void *restrict value, void *restrict data);

/* Creates a new, empty red-black tree */
struct rb_tree* rb_tree_create(void)
    __attribute__((returns_nonnull));

/* Sets the value destructor */
void rb_set_value_destructor(struct rb_tree *restrict tree, rb_callback destructor, void *restrict data)
    __attribute__((nonnull(1)));

/* Frees the resources held by a red-black tree */
void rb_tree_free(struct rb_tree *restrict) __attribute__((nonnull));

/* Inserts a pair (key, value) into the tree.
 *
 * If the key is already present, the associated value is changed
 */
void rb_insert(struct rb_tree *restrict, rb_key key, void *restrict value)
    __attribute__((nonnull));

/* Removes the binding associated with the key `key`. */
void rb_erase(struct rb_tree *restrict, rb_key key)
    __attribute__((nonnull));

/* Returns the value associated with the given key, NULL if none */
void* rb_get(struct rb_tree *restrict, rb_key key)
    __attribute__((nonnull));

/* Fires the callback on every (key, value) pair in the tree */
void rb_foreach(struct rb_tree *restrict, rb_callback, void *data)
    __attribute__((nonnull(1, 2)));

#endif /* !_RBT_H */
