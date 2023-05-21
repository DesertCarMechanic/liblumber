
#include <stdio.h>
#include <stdlib.h>

#include <sane.h>

#include "spec_common.h"
#include "../src/stack.h"


void stack_test_push_pop_start_print_error(void)
{ printf("LStack push_start or pop_start operation error"); }
void stack_test_push_pop_start_print_success(void)
{ printf("LStack push_start and pop_start operations successful"); }
enum RESULT stack_test_push_pop_start_func(void)
{
    u32 nr_of_randoms = rand() % (1<<8);
    i32 data[nr_of_randoms];
    // populate the array of random numbers
    for (u32 i=0; i<nr_of_randoms; i++) {
        data[i] = rand() % (1<<31); // numbers between 0 and 2^31
    }

    struct LStack **stack = l_stack_create();
    for (u32 i=0; i<nr_of_randoms; i++) {
        l_stack_push_start(stack, &data[i]);
    }

    u32 position = nr_of_randoms - 1;
    i32 *p_num = (i32 *)l_stack_pop_start(stack);
    while (p_num != NULL) {
        i32 n = *p_num;
        if (data[position] != n) {
            return RESULT_FAILURE;
        }
        position--;
        p_num = (i32 *)l_stack_pop_start(stack);
    }
    l_stack_destroy(stack);

    return RESULT_SUCCESS;
}


void stack_test_push_pop_end_print_error(void)
{ printf("LStack push_end or pop_end operation error"); }
void stack_test_push_pop_end_print_success(void)
{ printf("LStack push_end and pop_end operations successful"); }
enum RESULT stack_test_push_pop_end_func(void)
{
    u32 nr_of_randoms = rand() % (1<<4);
    uint data[nr_of_randoms];
    // populate the array with random numbers
    for (u32 i=0; i<nr_of_randoms; i++) {
        // data[i] = rand() % (1<<31);
        data[i] = rand() % (1<<5);
    }

    struct LStack **stack = l_stack_create();
    for (u32 i=0; i<nr_of_randoms; i++) {
        // printf("pushing on end: %d\n", data[i]);
        l_stack_push_end(stack, &data[i]);
    }

    u32 position = nr_of_randoms - 1;
    uint *p_num = (uint *)l_stack_pop_end(stack);
    while (p_num != NULL) {
        uint n = *p_num;
        // printf("popping from end: %d\n", n);
        if (n != data[position]) {
            return RESULT_FAILURE;
        }
        position--;
        p_num = (uint *)l_stack_pop_end(stack);
    }
    l_stack_destroy(stack);

    return RESULT_SUCCESS;
}


struct TestUnit stack_test_push_pop_start = {
    stack_test_push_pop_start_print_error,
    stack_test_push_pop_start_print_success,
    stack_test_push_pop_start_func,
};

struct TestUnit stack_test_push_pop_end = {
    stack_test_push_pop_end_print_error,
    stack_test_push_pop_end_print_success,
    stack_test_push_pop_end_func,
};

