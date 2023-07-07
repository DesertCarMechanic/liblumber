
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

