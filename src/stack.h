
// Simple stack implemented as a generic linked list.
struct LStack {
    void *data;
    struct LStack *next;
};

struct LStack **l_stack_create(void);
void l_stack_push_start(struct LStack **stack, void *value);
void *l_stack_pop_start(struct LStack **stack);
void l_stack_push_end(struct LStack **stack, void *value);
void *l_stack_pop_end(struct LStack **stack);
void l_stack_destroy(struct LStack **stack);
void l_stack_print(struct LStack **stack, void(*print_func)(void *value));
