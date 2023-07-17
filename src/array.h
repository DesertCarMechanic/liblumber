
#include <sane.h>

struct LArray {
    // size_t allocated_memory; // the total number of allocated bytes
    u64 allocated_blocks; // the total number of allocated blocks
    // `data` will expand or contract when necessary in increments of 
    // "memory_block_size" number of bytes
    size_t memory_block_size;
    size_t data_size; // how many bytes does each element contained in `data` take up
    void *data; // a pointer to the full block of memory containing all of the elements in contiguous order
    u64 length; // the number of elements contained in `data`
};

struct LArray *l_array_create(size_t data_size, size_t memory_block_size);
struct LArray *l_array_from_list(void *list, u64 length, size_t data_size, size_t memory_block_size);
void l_array_append(struct LArray *larray, void *thing);
void *l_array_pop(struct LArray *larray);
u64 l_array_length(struct LArray *larray);
void l_array_destroy(struct LArray *larray);
