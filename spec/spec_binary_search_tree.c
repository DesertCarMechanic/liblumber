

#include <stdio.h>
#include <stdlib.h>

#include <sane.h>

#include "spec_common.h"
#include "../src/binary_search_tree.h"
#include "spec_binary_search_tree.h"

i32 compare_int(void *a, void *b)
{
    u32 n1 = *(u32 *)a;
    u32 n2 = *(u32 *)b;

    if (n1 < n2) { 
        return 1;
    } else if (n1 > n2) {
        return -1;
    } else {
        return 0;
    }
}

void bst_test_insert_remove_print_error(void) 
{
    printf("binary search tree insert or remove error");
}

void bst_test_insert_remove_print_success(void) 
{
    printf("binary search tree insert and remove operations successful.");
}

enum RESULT bst_test_insert_remove_func(void)
{
    u32 nr_of_randoms = rand() % (1<<6);
    u32 data[nr_of_randoms];

    // populate the array with random ints
    for (u32 i=0; i<nr_of_randoms; i++) {
        data[i] = rand() % (1<<31);
    }

    // add them to the binary search tree
    struct LBSTNode **bst_tree = l_bst_create();
    for (u32 i=0; i<nr_of_randoms; i++) {
        l_bst_insert(bst_tree, &(data[i]), sizeof(u32), compare_int);
    }

    // remove all of them
    for (u32 i=0; i<nr_of_randoms; i++) {
        l_bst_remove(bst_tree, &data[i], compare_int);
    }

    // the tree should be empty now. if not, error
    if (*bst_tree != NULL) {
        return RESULT_FAILURE;
    }

    // struct LBSTIterator *bst_iter = l_bst_iterator_create(bst_tree);
    // struct LBSTNode *n = l_bst_iterator_next(bst_iter);
    // while (n != NULL) {
    //     printf("num: %d\n", *(u32*) n->data);
    //     n = l_bst_iterator_next(bst_iter);
    // }
    // l_bst_iterator_destroy(bst_iter);

    l_bst_destroy(bst_tree);

    return RESULT_SUCCESS;
}



struct TestUnit bst_test_insert_remove = {
    bst_test_insert_remove_print_error,
    bst_test_insert_remove_print_success,
    bst_test_insert_remove_func,
};




