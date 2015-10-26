#include "rbt.h"

#include <assert.h>
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

/* Creates a red-black tree node. */
static struct rb_node* node_create(rb_key key, void *value)
    __attribute__((returns_nonnull));

/* Deletes a node, calling the destructor callback, if any */
static void node_free(struct rb_tree *restrict tree, struct rb_node *restrict node)
    __attribute__((nonnull(1)));

/* Deletes the subtree rooted at `node` recursively, calling the destructor callback, if any */
static void node_free_recursively(struct rb_tree *restrict tree, struct rb_node *restrict node)
    __attribute__((nonnull(1)));

/* Performs a left rotation (`node` <-> `node->right`) and returns the new parent of `node`. */
static inline struct rb_node* rotate_left(struct rb_node *node)
    __attribute__((nonnull, returns_nonnull));

/* Performs a right rotation (`node` <-> `node->left`) and returns the new parent of `node`. */
static inline struct rb_node* rotate_right(struct rb_node *node)
    __attribute__((nonnull, returns_nonnull));

/* Finds a node with the given key. Returns NULL if none is found. */
static struct rb_node* find(const struct rb_tree *restrict tree, rb_key key)
    __attribute__((nonnull));

/* Checks whether `node` is red. */
static inline bool is_red(const struct rb_node *restrict node)
    __attribute__((nonnull, pure));

/* Flips the colours of `node` and its children. */
static inline void colour_flip(struct rb_node *restrict node)
    __attribute__((nonnull));

/* Performs one step of the restoration of red-black properties. */
static inline struct rb_node* move_red_right(struct rb_node *restrict node)
    __attribute__((nonnull, returns_nonnull));

/* Performs one step of the restoration of red-black properties. */
static inline struct rb_node* move_red_left(struct rb_node *restrict node)
    __attribute__((nonnull, returns_nonnull));

/* Performs one step of the restoration of red-black properties. */
static struct rb_node* fixup(struct rb_node *restrict node)
    __attribute__((nonnull, returns_nonnull));

/* Inserts a (key, value) pair into the subtree rooted at `node`.
 *
 * It assumes no value is already associated with this key.
 */
static struct rb_node* insert(struct rb_node *restrict node, rb_key key, void *restrict value)
    __attribute__((returns_nonnull));

/* Returns the node with the minimal key in the subtree rooted at `node`. */
static inline struct rb_node* min_node(struct rb_node *restrict node)
    __attribute__((nonnull, returns_nonnull));

/* Removes the binding associated with the key `key` from the subtree rooted at `node`.
 *
 * Returns the new root.
 */
static struct rb_node* delete(struct rb_tree *restrict tree, struct rb_node *restrict node, rb_key key)
    __attribute__((nonnull));

/* Performs a three-way comparison between rb_keys. */
static inline int compare(rb_key, rb_key)
    __attribute__((pure, const));

/* 
 * =================== Public functions ===================
 */

struct rb_tree* rb_tree_create(void) {
    struct rb_tree *tree = calloc(1, sizeof(struct rb_tree));
    if(!tree)
        fail("Unable to allocate memory for a red-black tree");
    return tree;
}

void rb_tree_free(struct rb_tree *restrict tree) {
    if(tree->root)
        node_free_recursively(tree, tree->root);
    free(tree);
}

void rb_set_value_destructor(struct rb_tree *restrict tree, rb_value_destructor destructor, void *restrict data) {
    tree->destructor = destructor;
    tree->destructor_data = data;
}

void* rb_get(struct rb_tree *restrict tree, rb_key key) {
    struct rb_node *restrict node = find(tree, key);
    return node ? node->value : NULL;
}

void rb_erase(struct rb_tree *restrict tree, rb_key key) {
    delete(tree, tree->root, key);
}

void rb_insert(struct rb_tree *restrict tree, rb_key key, void *restrict value) {
    tree->root = insert(tree->root, key, value);
}

/* 
 * =================== Private functions ===================
 */

static struct rb_node *node_create(rb_key key, void *value) {
    struct rb_node *node = calloc(1, sizeof(struct rb_node));
    if(!node)
        fail("Unable to allocate memory for a red-black tree node");

    node->key = key;
    node->value = value;
    node->red = true;
    return node;
}

static void node_free(struct rb_tree *restrict tree, struct rb_node *restrict node) {
    if(!node)
        return;

    if(tree->destructor)
        tree->destructor(tree, node->key, node->value, tree->destructor_data); 

    free(node);
}

static void node_free_recursively(struct rb_tree *restrict tree, struct rb_node *restrict node) {
    if(!node)
        return;

    node_free_recursively(tree, node->left);
    node_free_recursively(tree, node->right);
    node_free(tree, node);
}

static struct rb_node* find(const struct rb_tree *restrict tree, rb_key key) {
   struct rb_node *restrict node = tree->root; 

   while(node) {
       if(key < node->key)
           node = node->left;
       else if(node->key < key)
           node = node->right;
       else
           return node;
   }

   return NULL;
}

static inline bool is_red(const struct rb_node *node) {
    return node && node->red;
}

static inline struct rb_node* rotate_left(struct rb_node *node) {
    struct rb_node *aux = node->right;
    node->right = aux->left;
    aux->left = node;
    aux->red = node->red;
    node->red = true;
    return aux;
}

static inline struct rb_node* rotate_right(struct rb_node *node) {
    struct rb_node *aux = node->left;
    node->left = aux->right;
    aux->right = node;
    aux->red = node->red;
    node->red = true;
    return aux;
}

static inline void colour_flip(struct rb_node *node) {
    node->red = !node->red;
    node->left->red = !node->left->red;
    node->right->red = !node->right->red;
}

static inline struct rb_node* move_red_right(struct rb_node *node) {
    colour_flip(node);

    if(is_red(node->left->left)) {
        node = rotate_right(node);
        colour_flip(node);
    }

    return node;
}

static inline struct rb_node* move_red_left(struct rb_node *node) {
    colour_flip(node);

    if(is_red(node->right->left)) {
        node->right = rotate_right(node->right);
        node = rotate_left(node);
        colour_flip(node);
    }

    return node;
}

static struct rb_node* fixup(struct rb_node *node) {
    if(is_red(node->right))
        node = rotate_left(node);
    
    if(is_red(node->left) && is_red(node->left->left))
        node = rotate_right(node);
    
    if(is_red(node->left) && is_red(node->right))
        colour_flip(node);

    return node;
}

static struct rb_node* unlink_min(struct rb_node *node) {
    if(!node->left)
        return NULL;

    if(!is_red(node->left) && !is_red(node->left->left))
        node = move_red_left(node);

    node->left = unlink_min(node->left);

    return fixup(node);
}

static struct rb_node* insert(struct rb_node *node, rb_key key, void *value) {
    if(!node)
        return node_create(key, value);

    int r = compare(key, node->key);
    assert(r != 0);

    if(r < 0)
        node->left = insert(node->left, key, value);
    else
        node->right = insert(node->right, key, value);

    return fixup(node);
}

static inline struct rb_node* min_node(struct rb_node *node) {
    while(node->left)
        node = node->left;

    return node;
}

static struct rb_node* delete(struct rb_tree *tree, struct rb_node *node, rb_key key) {
    int r = compare(key, node->key);
    if(r < 0) {
        if(!is_red(node->left) && !is_red(node->left->left))
            node = move_red_left(node);
        node->left = delete(tree, node->left, key);
    }
    else {
        if(is_red(node->left))
            node = rotate_right(node);

        if(r == 0 && !node->right) {
            free(node);
            return NULL;
        }

        if(!is_red(node->right) && !is_red(node->right->left))
            node = move_red_right(node);

        if(r == 0) {
            struct rb_node *min = min_node(node->right);
            node->key = min->key;
            node->value = min->value;
            node->right = unlink_min(node->right);
            free(min);
        }
        else
            node->right = delete(tree, node->right, key);
    }
    
    return fixup(node);
}

static inline int compare(rb_key lhs, rb_key rhs) {
    if(lhs < rhs)
        return -1;
    else if(lhs == rhs)
        return 0;
    else
        return 1;
}
