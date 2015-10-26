#include "rbt.h"

#include "common.h"

#include <stdbool.h>
#include <stdlib.h>

/* Represents a node in a red-black tree. */
struct rb_node {
    struct rb_node *left, *right;
    bool red;

    rb_key key;
    void *value;
};

struct rb_tree {
    /* The root of the red black tree, NULL if none */
    struct rb_node *root;

    /* The destructor callback, NULL if none */
    rb_value_destructor destructor;

    /* The destructor callback data, NULL if none */
    void *destructor_data;
};

/*
 * =================== Private interface ===================
 */

/* Deletes a node, calling the destructor callback, if any */
static void rb_node_delete(struct rb_tree *restrict tree, struct rb_node *restrict node) {
    if(!node)
        return;

   if(tree->destructor)
      tree->destructor(tree, node->key, node->value, tree->destructor_data); 

   rb_node_delete(tree, node->left);
   rb_node_delete(tree, node->right);

   free(node);
}

/* 
 * =================== Public functions ===================
 */

struct rb_tree* rb_tree_new(void) {
    struct rb_tree *tree = calloc(1, sizeof(struct rb_tree));
    if(!tree)
        fail("Unable to allocate memory for a red-black tree");
    return tree;
}

void rb_tree_delete(struct rb_tree *restrict tree) {
    rb_node_delete(tree, tree->root);
    free(tree);
}

void rb_set_value_destructor(struct rb_tree *restrict tree, rb_value_destructor destructor, void *restrict data) {
    tree->destructor = destructor;
    tree->destructor_data = data;
}
