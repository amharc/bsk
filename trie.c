#include "trie.h"
#include "common.h"
#include "rbt.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

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

/* A value destructor for the red-black trees. */
static void rb_value_destructor(const struct rb_tree *restrict tree, rb_key key, void *restrict value, void *restrict data)
    __attribute__((nonnull(1, 3)));

struct trie_get_even_data {
    struct unbounded_string *current;
    struct trie_get_even_response result;
};

static void node_get_even(const struct trie_node *restrict node, struct trie_get_even_data *restrict data)
    __attribute__((nonnull));

static void node_get_even_foreach_callback(const struct rb_tree *restrict tree, rb_key key, void *restrict value, void *restrict data)
    __attribute__((nonnull(1, 3)));

/* 
 * =================== Public functions ===================
 */

struct trie* trie_create(void) {
    struct trie *trie = calloc(1, sizeof(struct trie));

    if(!trie)
        fail(WITH_ERRNO, "Unable to allocate memory for a TRIE");

    trie->root = node_create();

    return trie;
}

void trie_free(struct trie *trie) {
    node_free_recursively(trie->root);
    free(trie);
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

struct trie_get_even_response trie_get_even(struct trie *trie) {
    struct trie_get_even_data data = {
        .current = us_from_string(""),
        .result = {
            .word = NULL,
            .count = 0
        }
    };

    node_get_even(trie->root, &data);

    us_free(data.current);
    return data.result;
}

/* 
 * =================== Private functions ===================
 */


struct trie_node* node_create() {
    struct trie_node *node = calloc(1, sizeof(struct trie_node));
    if(!node)
        fail(WITH_ERRNO, "Unable to allocate memory for a TRIE node");

    node->children = rb_tree_create();
    rb_set_value_destructor(node->children, &rb_value_destructor, NULL);

    return node;
}

void node_free_recursively(struct trie_node *node) {
    if(!node)
        return;

    rb_tree_free(node->children);
    free(node);
}

void rb_value_destructor(const struct rb_tree *tree __attribute__((unused)),
        rb_key key __attribute__((unused)),
        void *value,
        void *data __attribute__((unused))) {
    node_free_recursively((struct trie_node *) value);
}

void node_get_even(const struct trie_node *node, struct trie_get_even_data *data) {
    if(node->counter > 0 && node->counter % 2 == 0 && !data->result.word) {
        data->result.count = node->counter;
        data->result.word = strndup(us_to_string(data->current), us_length(data->current));
        if(!data->result.word) {
            fail(WITH_ERRNO, "Unable to copy a string");
        }
    }

    if(!data->result.word) {
        rb_foreach(node->children, &node_get_even_foreach_callback, data);
    }
}

void node_get_even_foreach_callback(const struct rb_tree *tree __attribute__((unused)),
        rb_key key, void *_node, void *_data) {
    struct trie_get_even_data *data = _data;
    const struct trie_node *node = _node;

    us_push(data->current, key);
    node_get_even(node, data);
    us_pop(data->current);
}
