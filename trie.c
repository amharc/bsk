#include "trie.h"
#include "common.h"
#include "rbt.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

/* Represents a node in a TRIE */
struct trie_node {
    /* A map (char -> trie_node) of the children of the current node. */
    struct rb_tree *children;

    /* A counter indicating how many words end in this node. */
    size_t counter;
};

struct trie {
    struct trie_node *root;
};

/*
 * =================== Private interface ===================
 */

/* Creates a TRIE node. */
static struct trie_node* node_create()
    __attribute__((returns_nonnull));

/* Deletes a TRIE node recursively. */
static void node_free_recursively(struct trie_node *restrict node);

void rb_value_destructor(const struct rb_tree *restrict tree, rb_key key, void *restrict value, void *restrict data)
    __attribute__((nonnull(1, 3)));

/* 
 * =================== Public functions ===================
 */

struct trie* trie_create(void) {
    struct trie *trie = calloc(1, sizeof(struct trie));
    if(!trie)
        fail("Unable to allocate memory for a TRIE");

    trie->root = node_create();

    return trie;
}

void trie_free(struct trie *tree) {
    node_free_recursively(tree->root);
}

void trie_insert(struct trie *trie, const char *word, size_t length) {
    struct trie_node *node = trie->root;

    for(size_t index = 0; index < length; ++index) {
        struct trie_node *next = rb_get(node->children, word[index]);

        if(!next) {
            next = node_create();
            rb_insert(node->children, word[index], next);
        }

        node = next;
    }

    node->counter++;
}

/* 
 * =================== Private functions ===================
 */


struct trie_node* node_create() {
    struct trie_node *node = calloc(1, sizeof(struct trie_node));
    if(!node)
        fail("Unable to allocate memory for a TRIE node");

    node->children = rb_tree_create();
    rb_set_value_destructor(node->children, &rb_value_destructor, NULL);

    return node;
}

void node_free_recursively(struct trie_node *node) {
    if(!node)
        return;

    rb_tree_free(node->children);
}

void rb_value_destructor(const struct rb_tree *tree __attribute__((unused)),
        rb_key key __attribute__((unused)),
        void *value,
        void *data __attribute__((unused))) {
    node_free_recursively((struct trie_node *) value);
}
