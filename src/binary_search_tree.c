
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> // for memcpy

#include "stack.h"
#include "binary_search_tree.h"


struct LBSTIterator *l_bst_iterator_create(struct LBSTNode *bst_root)
{
    struct LBSTIterator *p_iter = malloc(sizeof(struct LBSTIterator));
    struct LStack **pp_stack = l_stack_create();

    p_iter->explored_left = false;
    p_iter->stack = pp_stack;

    l_stack_push_start(pp_stack, bst_root);
    return p_iter;
}

struct LBSTNode *l_bst_iterator_next(struct LBSTIterator *b) 
{
    struct LStack **stack = b->stack;
    struct LStack *last_pushed_link = (struct LStack *) *stack;
    if (last_pushed_link == NULL) return NULL;
    struct LBSTNode *last_node = (struct LBSTNode *) last_pushed_link->data;
    if (last_node == NULL) return NULL; // null node?? this should never execute

    if (b->explored_left) {
        goto exit;
    } else {
        struct LBSTNode *leftmost = last_node->left;
        if (leftmost == NULL) goto exit; // no left node, just return the last one in the stack

        while (leftmost != NULL) { // push all left-side nodes on the stack
            l_stack_push_start(stack, leftmost);
            last_node = leftmost;
            leftmost = leftmost->left;
        }
        goto exit; // pop the last one and return
    }

exit:
    struct LBSTNode *right = last_node->right;
    struct LBSTNode *target = l_stack_pop_start(stack);
    if (right != NULL) {
        l_stack_push_start(stack, right);
        b->explored_left = false;
    } else {
        b->explored_left = true;
    }

    return target;
}

void l_bst_iterator_destroy(struct LBSTIterator *b)
{
    l_stack_destroy(b->stack);
    free(b);
}

struct LBSTNode **l_bst_create(void)
{
    struct LBSTNode **structure_ptr = malloc(sizeof(struct LBSTNode *));
    *structure_ptr = NULL; // root element is NULL ptr to denote empty tree
    return structure_ptr;
}

void l_bst_insert(struct LBSTNode **root, void *data, size_t data_size, int(*compare)(void*, void*))
{
    if (root == NULL) return;
    struct LBSTNode *current = *root;
    struct LBSTNode *parent = NULL;

    while (current != NULL) {
        parent = current;
        int comparison = compare(current->data, data);
        if (comparison == 0) {
            return; // the element is already in the list, skip.
        } 
        if (comparison < 0) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    struct LBSTNode *new_node = malloc(sizeof(struct LBSTNode));
    void *d = malloc(data_size);
    memcpy(d, data, data_size);
    new_node->data = d;
    new_node->left = NULL;
    new_node->right = NULL;

    if (parent == NULL) {
        *root = new_node;
        return;
    }

    int parent_comparison = compare(parent->data, data);
    if (parent_comparison < 0) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

void _attach_a_to_leftmost_branch_of_b(struct LBSTNode *a, struct LBSTNode *b)
{
    struct LBSTNode *leftmost_of_b_parent = b;
    struct LBSTNode *leftmost_of_b = b->left;

    while (leftmost_of_b != NULL) {
        leftmost_of_b_parent = leftmost_of_b;
        leftmost_of_b = leftmost_of_b->left;
    }

    leftmost_of_b_parent->left = a;
}

void l_bst_remove(struct LBSTNode **root, void *value, int (*compare)(void*, void*)) 
{
    struct LBSTNode *parent = NULL;
    struct LBSTNode *target = *root;
    bool last_branch_direction;

    while (true) {
        if (target == NULL) return; // we couldn't find the value

        int comparison = compare(target->data, value);
        if (comparison == 0) {
            break; // we found the node to remove
        }

        parent = target;
        if (comparison < 0) {
            target = target->left;
            last_branch_direction = false;
        } else {
            target = target->right;
            last_branch_direction = true;
        }
    }

    struct LBSTNode *tleft = target->left;
    struct LBSTNode *tright = target->right;

    free(target->data); // make sure to also free the data contained in the node
    free(target); // free the node itself

    if (parent == NULL) {
        if ((tleft == NULL) && (tright == NULL)) {
            // the root node is the only node and we removed it
            *root = NULL;
        } else if ((tleft == NULL) && (tright != NULL)) {
            *root = tright;
        } else if ((tleft != NULL) && (tright == NULL)) {
            *root = tleft;
        } else { // ((tleft != NULL) && (tright != NULL)) {
            _attach_a_to_leftmost_branch_of_b(tleft, tright);
            *root = tright;
        }
    } else {
        if ((tleft == NULL) && (tright == NULL)) {
            if (last_branch_direction == false) {
                // target is left branch of the parent
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else if ((tleft == NULL) && (tright != NULL)) {
            if (last_branch_direction == false) {
                // target is left branch of the parent
                parent->left = tright;
            } else {
                parent->right = tright;
            }
        } else if ((tleft != NULL) && (tright == NULL)) {
            if (last_branch_direction == false) {
                // target is left branch of the parent
                parent->left = tleft;
            } else {
                parent->right = tleft;
            }
        } else { // ((tleft != NULL) && (tright != NULL)) {
            _attach_a_to_leftmost_branch_of_b(tleft, tright);
            if (last_branch_direction == false) {
                // target is left branch of the parent
                parent->left = tright;
            } else {
                parent->right = tright;
            }
        }
    }
}

void l_bst_destroy(struct LBSTNode **bst_root)
{
    if (bst_root == NULL) return;
    struct LBSTIterator *p_iter = l_bst_iterator_create(*bst_root);
    struct LBSTNode *p_node = l_bst_iterator_next(p_iter);
    while (p_node != NULL) {
        free(p_node->data);
        free(p_node);
        p_node = l_bst_iterator_next(p_iter);
    }
    l_bst_iterator_destroy(p_iter);
    free(bst_root);
}

void l_bst_print(struct LBSTNode *root, void (*print_func)(void*))
{
    if (root == NULL) return;
    print_func(root->data);
    l_bst_print(root->left, print_func);
    l_bst_print(root->right, print_func);
}



