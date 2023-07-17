
#include <stddef.h>
#include <sane.h>

struct LBST {
    struct LBSTNode *root;
    int (*compare)(void *, void *);
    size_t data_size;
};

struct LBSTNode {
    void *data;
    struct LBSTNode *left;
    struct LBSTNode *right;
};

struct LBSTIterator {
    struct LStack **stack;
    bool explored_left;
};

struct LBST *l_bst_create(int(*compare)(void *, void *), size_t data_size);
void l_bst_insert(struct LBST *bst, void *value);
void l_bst_remove(struct LBST *bst, void *value);
void l_bst_destroy(struct LBST *bst);
// void l_bst_print(struct LBSTNode *root, void (*print_func)(void*));

struct LBSTIterator *l_bst_iterator_create(struct LBST *a);
struct LBSTNode *l_bst_iterator_next(struct LBSTIterator *b);
void l_bst_iterator_destroy(struct LBSTIterator *b);
