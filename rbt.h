#ifndef _RBT_H
#define _RBT_H

#include "common.h"

#include <stdbool.h>

/* The type of keys in the red-black tree */
typedef char rb_key;

/* An opaque type representing the red-black tree */
struct rb_tree;

/* A value destructor function
 *
 * Arguments:
 *   - the red black tree in which the value is present
 *   - the key with which it is associated
 *   - the value to be removed
 *   - user-provided data
 */
typedef void (*rb_value_destructor)(struct rb_tree *restrict tree, rb_key key,
            void *restrict value, void *restrict data);

/* Creates a new, empty red-black tree */
struct rb_tree* rb_tree_create(void) __attribute__((returns_nonnull));

/* Sets the value destructor */
void rb_set_value_destructor(struct rb_tree *restrict tree, rb_value_destructor destructor, void *restrict data)
    __attribute__((nonnull(1)));

/* Frees the resources held by a red-black tree */
void rb_tree_free(struct rb_tree *restrict) __attribute__((nonnull));

/* Inserts a pair (key, value) into the tree.
 *
 * If the key is already present, the associated value is changed
 */
void rb_insert(struct rb_tree *restrict, rb_key key, void *restrict value) __attribute__((nonnull));

/* Removes the binding associated with the key `key`. */
void rb_erase(struct rb_tree *restrict, rb_key key) __attribute__((nonnull));

/* Returns the value associated with the given key, NULL if none */
void* rb_get(struct rb_tree *restrict, rb_key key) __attribute__((nonnull));

#endif /* !_RBT_H */
