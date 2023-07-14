
#include <stdlib.h>
#include <sane.h>

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
        int comparison = compare(value, tool_to_null->data);
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
        int comparison = compare(value, tool_to_null->data);
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

    bool is_first_run = TRUE;
    bool rbt_properties_satisfied = FALSE;
    while (rbt_properties_satisfied != TRUE) {

        if (parent == NULL) { // "new_node" is root
            new_node->color = RBT_COLOR_BLACK; // root node is always black
            *tree = parent;
            return;
        }

        if (is_first_run) {
            // on the first run of the algorithm we want to attach the new_node
            // but on subsequent loops we don't
            int comparison = compare(value, parent->data);
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
                parent->data,
                grandparent->data
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
            is_first_run = FALSE;
            // from here the code will recursively loop until it gets to the
            // root of the tree and fixes any inconsistency
        } else {
            // grandparent is black, uncle is black, parent is red, node is red

            int node_parent_relationship = compare(new_node->data, parent->data);
            int parent_grandparent_relationship = compare(parent->data, grandparent->data);

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
                            grandparent->data,
                            grandgrandparent->data
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
                            grandparent->data,
                            grandgrandparent->data
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
            rbt_properties_satisfied = TRUE;
        }
    }
}


void l_rbt_delete(struct LRBTNode **tree, void *value, int(*compare)(void *, void *))
{
    if (tree == NULL) return;
    struct LRBTNode *target = *tree;

    while (TRUE) {

        if (target == NULL) return; // we couldn't find the node to delete

        int comparison = compare(value, target->data);
        if (comparison == 0) {
            break; // we found the node we want to delete
        }
        if (comparison < 0) { // value < target->data
            target = target->left;
        } else { // value > target->data
            target = target->right;
        }
    }

    if ((target->left != NULL) && (target->right != NULL)) {
        // we're trying to delete a node with two children. We need to turn
        // this into a scenario where we only have to delete a node with one
        // or no children. To do this, replace the node we want to delete with
        // the rightmost node of the left branch. Then, we'll delete the
        // rightmost node of the left branch, which will always only have one
        // or no children

        struct LRBTNode *rightmost_node = target->left;
        struct LRBTNode *tool_to_null = rightmost_node->right;
        while (tool_to_null != NULL) {
            rightmost_node = tool_to_null;
            tool_to_null = tool_to_null->right;
        }

        target->data = rightmost_node->data;
        target = rightmost_node;

        // struct LRBTNode *target_left = target->left;
        // struct LRBTNode *rightmost_node_parent = rightmost_node->parent;
        // struct LRBTNode *target_parent = target->parent;
        // if (rightmost_node->left != NULL) {
        //     target->left = rightmost_node->left;
        // }
        // rightmost_node->left = target_left;
        // rightmost_node->right = target->right;
        // rightmost_node->parent = target->parent;

        // if (target_parent != NULL) {
        //     int comparison = compare(target->data, target_parent->data);
        //     if (comparison < 0) {
        //         target_parent->left = rightmost_node;
        //     } else { // this should always be 1
        //         target_parent->right = rightmost_node;
        //     }
        // } else { // node we want to delete is the root node
        //     rightmost_node->color = RBT_COLOR_BLACK; // root node is always black
        // }

        // rightmost_node_parent->right = target;
        // target->parent = rightmost_node_parent;
    }

    // if the code got here, `target` can only have one or no children
    struct LRBTNode *target_parent = target->parent;
    int target_parent_comparison = compare(target->data, target_parent->data);
    if (target->color == RBT_COLOR_RED) {
        // if target is red, it cannot have any children, so just delete it.
        if (target_parent_comparison < 0) {
            target_parent->left = NULL;
        } else { // this should always be 1
            target_parent->right = NULL;
        }
        free(target);
        return;
    }

    struct LRBTNode *target_child = NULL;
    int child_target_comparison = 0;
    if (target->left != NULL) {
        target_child = target->left;
        child_target_comparison = -1;
    } else {
        target_child = target->right;
        child_target_comparison = 1;
    }

    if (target_child->color == RBT_COLOR_RED) {
        if (target_parent_comparison < 0) {
            target_parent->left = target_child;
        } else {
            target_parent->right = target_child;
        }
        target_child->color = RBT_COLOR_BLACK;
        free(target);
        return;
    }

    // by this point both the target and its child are black, which means more
    // work for us

}


void l_rbt_destroy(struct LRBTNode **rbt)
{
    if (rbt == NULL) return;
    free(rbt);
}





