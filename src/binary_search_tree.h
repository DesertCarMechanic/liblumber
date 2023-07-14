
#include <stddef.h>
#include <sane.h>

struct LBSTNode {
    void *data;
    struct LBSTNode *left;
    struct LBSTNode *right;
};

struct LBSTIterator {
    struct LStack **stack;
    bool explored_left;
};


struct LBSTNode **l_bst_create(void);
void l_bst_insert(struct LBSTNode **root, void *data, size_t data_size, int(*compare)(void*, void*));
void l_bst_remove(struct LBSTNode **root, void *value, int (*compare)(void*, void*));
void l_bst_destroy(struct LBSTNode **root);
void l_bst_print(struct LBSTNode *root, void (*print_func)(void*));

struct LBSTIterator *l_bst_iterator_create(struct LBSTNode *a);
struct LBSTNode *l_bst_iterator_next(struct LBSTIterator *b);
void l_bst_iterator_destroy(struct LBSTIterator *b);
