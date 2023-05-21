
#include "stack.h"
#include <stdlib.h>
#include <sane.h>
#include <stdio.h>

struct LStack **l_stack_create(void)
{
    struct LStack **stack = malloc(sizeof(struct LStack *));
    *stack = NULL; // MUST be NULL ptr to denote end of stack
    return stack;
}

// push a value on to the beginning of the stack. O(1) time
void l_stack_push_start(struct LStack **stack, void *value)
{
    struct LStack *new_link = malloc(sizeof(struct LStack));
    new_link->data = value;
    new_link->next = *stack;
    *stack = new_link;
}

// pop a value from the beginning of the stack. O(1) time
void *l_stack_pop_start(struct LStack **stack)
{
    void *data = NULL;

    struct LStack *top = *stack;
    if (top == NULL) return NULL;
    *stack = top->next;
    data = top->data;

    free(top);
    return data;
}

// push a value on to the end of the stack; O(n) time
void l_stack_push_end(struct LStack **stack, void *value)
{
    if (stack == NULL) return;
    // get the last value in the stack
    struct LStack *last_link = NULL;
    struct LStack *tool_to_null = *stack;
    while (tool_to_null != NULL) {
        last_link = tool_to_null;
        tool_to_null = tool_to_null->next;
    }

    struct LStack *new_link = malloc(sizeof(struct LStack));
    new_link->data = value;
    new_link->next = NULL;
    if (last_link == NULL) {
        *stack = new_link; // no element in the stack, set it as first
    } else {
        // printf("last_link: %p\n", last_link);
        last_link->next = new_link;
    }
}

// pop a value from the end of the stack; O(n) time
void *l_stack_pop_end(struct LStack **stack)
{
    if (stack == NULL) return NULL;

    struct LStack *penultimate_link = NULL;
    struct LStack *last_link = NULL;
    struct LStack *tool_to_null = *stack;
    while (tool_to_null != NULL) {
        penultimate_link = last_link;
        last_link = tool_to_null;
        tool_to_null = tool_to_null->next; // segfault
    }
    if (last_link == NULL) return NULL;

    void *data = last_link->data;
    free(last_link);
    if (penultimate_link == NULL) {
        // there's only one link left, so set it to NULL because we're 
        // returning its data now
        *stack = NULL;
    } else { // there's at least two links left so return the last one
        // and set the one before to point to NULL
        penultimate_link->next = NULL;
    }
    return data;
}

void l_stack_destroy(struct LStack **stack)
{
    if (stack == NULL) return;
    struct LStack *s = *stack;
    while (s != NULL) {
        struct LStack *next = s->next;
        free(s);
        s = next;
    }
    free(stack);
}

void l_stack_print(struct LStack **stack, void(*print_func)(void *value))
{
    printf("LStack:");
    struct LStack *s = *stack;
    for (u64 i=0; s != NULL; i++) {
        printf("\t%lu:", i);
        print_func(s->data);
        printf("\n");
        s = s->next;
    }
}
