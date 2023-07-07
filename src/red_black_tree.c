
#include <stdlib.h>
#include <stdbool.h>

#include "red_black_tree.h"

struct LRBTNode **l_rbt_create(void)
{
    struct LRBTNode **rbt = malloc(sizeof(struct LRBTNode *));
    *rbt = NULL; // the first node is NULL, to represent an empty tree
    return rbt;
}


struct LRBTNode *l_rbt_search(struct LRBTNode **tree, void *value, size_t value_size, int(*compare)(void *, void *))
{
    if (tree == NULL) return NULL;
    struct LRBTNode *tool_to_null = *tree;
    while (tool_to_null != NULL) {
        int comparison = compare(tool_to_null->data, value);
        if (comparison == 0) return tool_to_null;
        if (comparison < 0) { // second argument smaller than first argument
            tool_to_null = tool_to_null->left;
        } else {
            tool_to_null = tool_to_null->right;
        }
    }
    return NULL;
}

// TODO: make this return some sort of failure/success object.
// TODO: make the compare function take the arguments in opposite order
// Also, make the success object contain the address to the newly inserted node
void l_rbt_insert(struct LRBTNode **tree, void *value, size_t value_size, int(*compare)(void *, void *))
{
    if (tree == NULL) return; // tree is NULL?? TODO: add a debug print here
    struct LRBTNode *parent = NULL;
    struct LRBTNode *tool_to_null = *tree;
    while (tool_to_null != NULL) {
        parent = tool_to_null;
        int comparison = compare(tool_to_null->data, value);
        if (comparison == 0) return; // element is already in the tree
        if (comparison < 0) {
            tool_to_null = tool_to_null->left;
        } else {
            tool_to_null = tool_to_null->right;
        }
    }

    struct LRBTNode *new_node = malloc(sizeof(struct LRBTNode)); // TODO: add error checking for malloc
    new_node->parent = parent;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->data = value;
    new_node->color = RBT_COLOR_RED;

    struct LRBTNode *grandparent = NULL;
    struct LRBTNode *uncle = NULL;

    bool is_first_run = true;
    bool rbt_properties_satisfied = false;
    while (rbt_properties_satisfied != true) {

        if (parent == NULL) { // "new_node" is root
            new_node->color = RBT_COLOR_BLACK; // root node is always black
            *tree = parent;
            return;
        }

        if (is_first_run) {
            // on the first run of the algorithm we want to attach the new_node
            // but on subsequent loops we don't
            int comparison = compare(parent->data, value);
            if (comparison == 0) return; //TODO: only add this code if a debug symbol is defined
            if (comparison < 0) {
                parent->left = new_node;
            } else {
                parent->right = new_node;
            }
        }

        if (parent->color == RBT_COLOR_BLACK) return; // tree properties preserved. nothing else to do

        // from this point onward the parent is certainly red
        grandparent = parent->parent;
        uncle = NULL;

        if (grandparent == NULL) {
            // there's no grandparent, so the parent is the root node.
            // color it black and return;
            parent->color = RBT_COLOR_BLACK;
            return;
        } else {
            int parent_grandparent_relationship = compare(
                grandparent->data,
                parent->data
            );

            // TODO: add the below line only if debug symbol is defined because
            // this should never happen
            if (parent_grandparent_relationship == 0) {}

            if (grandparent != NULL) {
                if (parent_grandparent_relationship < 0) { // parent is left of grandparent
                    uncle = grandparent->right;
                } else {
                    uncle = grandparent->left;
                }
            }
        }

        if (uncle != NULL && uncle->color == RBT_COLOR_RED) {
            // parent and uncle are red
            parent->color = RBT_COLOR_BLACK;
            uncle->color = RBT_COLOR_BLACK;
            grandparent->color = RBT_COLOR_RED;

            new_node = grandparent;
            parent = new_node->parent;
            is_first_run = false;
            // from here the code will recursively loop until it gets to the
            // root of the tree and fixes any inconsistency
        } else {
            // grandparent is black, uncle is black, parent is red, node is red

            int node_parent_relationship = compare(parent->data, new_node->data);
            int parent_grandparent_relationship = compare(grandparent->data, parent->data);

            if (parent_grandparent_relationship < 0) {
                if (node_parent_relationship > 0) {
                    // grandparent -left> parent -right> node

                    struct LRBTNode *node_left_temp = new_node->left;
                    struct LRBTNode *temp = NULL;

                    // rotate node counterclockwise compared to parent
                    new_node->left = parent;
                    parent->right = node_left_temp;
                    grandparent->left = new_node;

                    // swap the nodes on the stack so we dont get confused
                    temp = parent;
                    parent = new_node;
                    new_node = temp;

                    // update the parent pointers
                    node_left_temp->parent = new_node;
                    new_node->parent = parent;
                    parent->parent = grandparent;

                    // node is now left child of parent
                    node_parent_relationship = -1;
                }
                if (node_parent_relationship < 0) {
                    // grandparent -left> parent -left> node
                    struct LRBTNode *grandgrandparent = grandparent->parent;
                    struct LRBTNode *parent_right_temp = parent->right;

                    // rotate grandparent clockwise compared to parent
                    parent->right = grandparent;
                    grandparent->left = parent_right_temp;
                    if (grandgrandparent != NULL) {
                        int grandgrandparent_grandparent_relationship = compare(
                            grandgrandparent->data,
                            grandparent->data
                        );
                        // TODO: only add the line below if debug symbol is defined
                        if (grandgrandparent_grandparent_relationship == 0) {};

                        if (grandgrandparent_grandparent_relationship < 0) {
                            grandgrandparent->left = parent;
                        } else {
                            grandgrandparent->right = parent;
                        }
                    }

                    // update parent pointers
                    parent->parent = grandgrandparent;
                    grandparent->parent = parent;
                    parent_right_temp->parent = grandparent;

                    // fix colors
                    parent->color = RBT_COLOR_BLACK;
                    grandparent->color = RBT_COLOR_RED;
                }
            } else {
                if (node_parent_relationship < 0) {
                    // grandparent -right> parent -left> node

                    struct LRBTNode *node_right_temp = new_node->right;
                    struct LRBTNode *temp = NULL;

                    // rotate node clockwise compared to parent
                    new_node->right = parent;
                    parent->left = node_right_temp;
                    grandparent->right = new_node;

                    // swap the nodes on the stack so we dont get confused in the next step
                    temp = parent;
                    parent = new_node;
                    new_node = temp;

                    // update the parent pointers
                    node_right_temp->parent = new_node;
                    new_node->parent = parent;
                    parent->parent = grandparent;

                    // node is now right child of parent
                    node_parent_relationship = 1;
                }
                if (node_parent_relationship > 0) {
                    // grandparent -right> parent -right> node
                    struct LRBTNode *grandgrandparent = grandparent->parent;
                    struct LRBTNode *parent_left_temp = parent->left;

                    // rotate grandparent counterclockwise compared to parent
                    parent->left = grandparent;
                    grandparent->right = parent_left_temp;
                    if (grandgrandparent != NULL) {
                        int grandgrandparent_grandparent_relationship = compare(
                            grandgrandparent->data,
                            grandparent->data
                        );
                        // TODO: only add the line below if debug symbol is defined
                        if (grandgrandparent_grandparent_relationship == 0) {};
                        if (grandgrandparent_grandparent_relationship < 0) {
                            grandgrandparent->left = parent;
                        } else {
                            grandgrandparent->right = parent;
                        }
                    }

                    // update parent pointers
                    parent_left_temp->parent = grandparent;
                    grandparent->parent = parent;
                    parent->parent = grandgrandparent;

                    // fix colors
                    parent->color = RBT_COLOR_BLACK;
                    grandparent->color = RBT_COLOR_RED;
                }
            }
            rbt_properties_satisfied = true;
        }
    }
}

void l_rbt_destroy(struct LRBTNode **rbt)
{
    if (rbt == NULL) return;
    free(rbt);
}


