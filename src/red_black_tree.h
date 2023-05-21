

enum RBT_COLOR {
    RBT_COLOR_RED,
    RBT_COLOR_BLACK
}

struct LRBTNode {
    struct LRBTNode *left;
    struct LRBTNode *right;
    struct LRBTNode *parent;
    RBT_COLOR color;
    void *data; // data the node points to
}


