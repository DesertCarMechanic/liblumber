
#include <stdio.h>
#include <stdlib.h>
#include <sane.h>

#include "stack.h"
#include "memtools.h"
#include "binary_search_tree.h"


struct LBSTIterator *l_bst_iterator_create(struct LBST *bst)
{
    struct LBSTIterator *p_iter = malloc(sizeof(struct LBSTIterator));
    struct LStack **pp_stack = l_stack_create();
    p_iter->explored_left = FALSE;
    p_iter->stack = pp_stack;
    if (bst == NULL) return p_iter;

    struct LBSTNode *bst_root = bst->root;
    if (bst_root == NULL) return p_iter;
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
        b->explored_left = FALSE;
    } else {
        b->explored_left = TRUE;
    }

    return target;
}

void l_bst_iterator_destroy(struct LBSTIterator *b)
{
    l_stack_destroy(b->stack);
    free(b);
}

struct LBST *l_bst_create(int(*compare_func)(void *, void *), size_t data_size)
{
    struct LBST *bst = malloc(sizeof(struct LBST));
    bst->root = NULL;
    bst->compare = compare_func;
    bst->data_size = data_size;
    return bst;
}

void l_bst_insert(struct LBST *bst, void *value)
{
    if (bst == NULL) return;
    int(*compare)(void *, void *) = bst->compare;
    size_t data_size = bst->data_size;
    struct LBSTNode *current = bst->root;
    struct LBSTNode *parent = NULL;

    while (current != NULL) {
        parent = current;
        int comparison = compare(value, current->data);
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
    void *node_data = malloc(data_size);
    l_mem_copy_blind(node_data, value, data_size);
    new_node->data = node_data;
    new_node->left = NULL;
    new_node->right = NULL;

    if (parent == NULL) {
        bst->root = new_node;
        return;
    }

    int parent_comparison = compare(value, parent->data);
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

void l_bst_remove(struct LBST *bst, void *value)
{
    if (bst == NULL) return;
    int (*compare)(void *, void *) = bst->compare;
    struct LBSTNode *parent = NULL;
    struct LBSTNode *target = bst->root;
    bool last_branch_direction;

    while (TRUE) {
        if (target == NULL) return; // we couldn't find the value

        int comparison = compare(value, target->data);
        if (comparison == 0) {
            break; // we found the node to remove
        }

        parent = target;
        if (comparison < 0) {
            target = target->left;
            last_branch_direction = FALSE;
        } else {
            target = target->right;
            last_branch_direction = TRUE;
        }
    }

    struct LBSTNode *tleft = target->left;
    struct LBSTNode *tright = target->right;

    free(target->data); // make sure to also free the data contained in the node
    free(target); // free the node itself

    if (parent == NULL) {
        if ((tleft == NULL) && (tright == NULL)) {
            // the root node is the only node and we removed it
            bst->root = NULL;
        } else if ((tleft == NULL) && (tright != NULL)) {
            bst->root = tright;
        } else if ((tleft != NULL) && (tright == NULL)) {
            bst->root = tleft;
        } else { // ((tleft != NULL) && (tright != NULL)) {
            _attach_a_to_leftmost_branch_of_b(tleft, tright);
            bst->root = tright;
        }
    } else {
        if ((tleft == NULL) && (tright == NULL)) {
            if (last_branch_direction == FALSE) {
                // target is left branch of the parent
                parent->left = NULL;
            } else {
                parent->right = NULL;
            }
        } else if ((tleft == NULL) && (tright != NULL)) {
            if (last_branch_direction == FALSE) {
                // target is left branch of the parent
                parent->left = tright;
            } else {
                parent->right = tright;
            }
        } else if ((tleft != NULL) && (tright == NULL)) {
            if (last_branch_direction == FALSE) {
                // target is left branch of the parent
                parent->left = tleft;
            } else {
                parent->right = tleft;
            }
        } else { // ((tleft != NULL) && (tright != NULL)) {
            _attach_a_to_leftmost_branch_of_b(tleft, tright);
            if (last_branch_direction == FALSE) {
                // target is left branch of the parent
                parent->left = tright;
            } else {
                parent->right = tright;
            }
        }
    }
}

void l_bst_destroy(struct LBST *bst)
{
    if (bst == NULL) return;
    struct LBSTIterator *p_iter = l_bst_iterator_create(bst);
    struct LBSTNode *p_node = l_bst_iterator_next(p_iter);
    while (p_node != NULL) {
        free(p_node->data);
        free(p_node);
        p_node = l_bst_iterator_next(p_iter);
    }
    l_bst_iterator_destroy(p_iter);
    free(bst);
}

// void l_bst_print(struct LBST *root, void (*print_func)(void*))
// {
//     if (root == NULL) return;
//     print_func(root->data);
//     l_bst_print(root->left, print_func);
//     l_bst_print(root->right, print_func);
// }


