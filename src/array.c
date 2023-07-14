
#include <stdlib.h> // for malloc, realloc
#include <stdio.h> // for printf

#include <sane.h>
#include "array.h"
#include "memtools.h"


// when the data structure runs out of memory the memory grows in increments 
// of "memory_block_size"
// TODO: add error checking for malloc and maybe also if memory_block_size < data_size
struct LArray *l_array_create(size_t data_size, size_t memory_block_size)
{
    if (memory_block_size < data_size) {
        printf("memory_block_size should be larger than data_size.\n");
        printf("Defaulting memory_block_size to 16 * data_size.\n");
        memory_block_size = data_size * 16;
    }

    struct LArray *larray = malloc(sizeof(struct LArray)); // TODO: add error checking

    larray->allocated_blocks = 0;
    larray->memory_block_size = memory_block_size;
    larray->data = NULL; // by default the data points to nothing
    larray->data_size = data_size;
    larray->length = 0;

    return larray;
}

struct LArray *l_array_from_list(void *list, u64 length, size_t data_size, size_t memory_block_size)
{
    if (memory_block_size < data_size) {
        printf("memory_block_size should be larger than data_size.\n");
        printf("Defaulting memory_block_size to 16 * data_size.\n");
        memory_block_size = data_size * 16;
    }

    size_t necessary_nr_of_blocks = ((length * data_size) / memory_block_size) + 1;

    struct LArray *larray = malloc(sizeof(struct LArray)); // TODO: add error checking

    larray->allocated_blocks = necessary_nr_of_blocks;
    larray->memory_block_size = memory_block_size;
    larray->data = list;
    larray->data_size = data_size;
    larray->length = length;

    return larray;
}


// TODO: add error checking for malloc
void l_array_append(struct LArray *larray, void *thing)
{
    // printf("length before appending: %lu\n", larray->length);
    if (larray->length == 0) {
        larray->data = malloc(larray->memory_block_size); // FIXME: add error checking
        l_mem_copy_blind(larray->data, thing, larray->data_size);
        larray->allocated_blocks = 1;
        larray->length = 1;
        return;
    }

    size_t necessary_memory = (larray->length + 1) * larray->data_size;
    if (larray->allocated_blocks * larray->memory_block_size < necessary_memory) {
        larray->allocated_blocks++;
        larray->data = realloc(larray->data, larray->allocated_blocks * larray->memory_block_size); // FIXME: add error checking
    }

    byte8 *target_address = (byte8 *)larray->data;
    target_address += larray->length * larray->data_size;
    l_mem_copy_blind((void *)target_address, thing, larray->data_size);
    larray->length++;
}

// get the last element from a contiguous array of elements
// the memory containing the returned element must be freed by the user after use
void *l_array_pop(struct LArray *larray)
{
    // printf("larray pop length: %lu\n", larray->length);
    if (larray->length == 0) return NULL;

    void *thing_memory = malloc(larray->data_size);
    byte8 *thing_address = (byte8 *)larray->data;
    thing_address += larray->data_size * (larray->length - 1);
    l_mem_copy_blind(thing_memory, (void *)thing_address, larray->data_size);

    size_t memory_occupied = larray->data_size * larray->length;
    size_t memory_allocated = larray->allocated_blocks * larray->memory_block_size;
    // printf("memory_occupied - larray->data_size: %lu %lu\n", memory_occupied, larray->data_size);
    // printf("memory_allocated - larray->memory_block_size: %lu %lu\n", memory_allocated, larray->memory_block_size);
    if (memory_occupied - larray->data_size <= memory_allocated - larray->memory_block_size) {
        // can we de-allocate a block of memory after removing this last item?
        larray->data = realloc(larray->data, (larray->allocated_blocks - 1) * larray->memory_block_size);
        larray->allocated_blocks--;
    }
    // if (larray->length == 1) { 
    //     larray->data = NULL; 
    //     free(larray->data);
    //     larray->allocated_blocks = 0;
    // }
    larray->length--;

    return thing_memory; // the user has to de-allocate this once he used it
}

u64 l_array_length(struct LArray *larray)
{
    if (larray == NULL) return 0; // TODO: maybe add some sort of error checking here
    return larray->length;
}

void l_array_destroy(struct LArray *larray)
{
    if (larray->length != 0) {
        free(larray->data);
    }
    free(larray);
}


