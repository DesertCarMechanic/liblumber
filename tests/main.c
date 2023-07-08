
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// #include <pthread.h>

// #define F_MEMORY_DEBUG // must be defined before including <forge.h>
// #include <forge.h> // for mem debugging
#include <sane.h>

#include "test_common.h"
#include "test_binary_search_tree.h"
#include "test_stack.h"

void run_test(struct TestUnit *t)
{
    enum RESULT r = t->test();
    if (r == RESULT_SUCCESS) {
        printf("Success: ");
        t->print_success();
        printf("\n");
    } else {
        printf("Error: ");
        t->print_error();
        printf("\n");
    }
}

// void mutex_lock(void *mutex) 
// {
//     pthread_mutex_lock((pthread_mutex_t *)mutex);
// }
// void mutex_unlock(void *mutex)
// {
//     pthread_mutex_unlock((pthread_mutex_t *)mutex);
// }

i32 main() {

    // pthread_mutex_t mutex;
    // pthread_mutex_init(&mutex, NULL);
    // f_debug_memory_init(mutex_lock, mutex_unlock, (void *)mutex);

    srand(TEST_SEED); // TEST_SEED defined in Makefile

    struct TestUnit *tests[] = {
        &bst_test_insert_remove,
        &stack_test_push_pop_start,
        &stack_test_push_pop_end,
        NULL,
    };

    u32 i;
    struct TestUnit *current_test = tests[0];
    for(i=1; current_test != NULL; i++) {
        run_test(current_test);
        current_test = tests[i];
    }

    // char *thing = malloc(sizeof(char) * 20);
    // size_t size = malloc_usable_size(thing);
    // printf("size of unsigned ints: %zu\n", sizeof(unsigned int));
    // printf("malloc usable size: %zu\n", size);

    // f_debug_mem_print(0);

    return 0;
}
