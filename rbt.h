#ifndef _RBT_H
#define _RBT_H

#include "common.h"

/* The type of keys in the red-black tree */
typedef char rb_key;

/* An opaque type representing the red-black tree */
struct rb_tree;

/* Creates a new, empty red-black tree */
struct rb_tree* rb_tree_new();

/* Frees the resources held by a red-black tree */
void rb_tree_delete(struct rb_tree *restrict) NONNULL;

/* Inserts a pair (key, value) into the tree.
 *
 * If the key is already present, the associated value is changed
 */
void rb_insert(struct rb_tree *restrict, rb_key key, void *restrict value) NONNULL;

/* Returns the value associated with the given key, NULL if none */
void* rb_access(struct rb_tree *restrict, rb_key key) NONNULL;

#endif /* !_RBT_H */
