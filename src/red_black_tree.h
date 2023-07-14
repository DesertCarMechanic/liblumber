
enum RBT_COLOR {
    RBT_COLOR_BLACK,
    RBT_COLOR_RED,
};

struct LRBTNode {
    struct LRBTNode *parent;
    struct LRBTNode *left;
    struct LRBTNode *right;
    void *data; // data the node points to
    enum RBT_COLOR color;
};

void l_rbt_insert(struct LRBTNode **tree, void *value, size_t value_size, int(*compare)(void *, void *));
void l_rbt_delete(struct LRBTNode **tree, void *value, int(*compare)(void *, void *));
