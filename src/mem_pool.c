
#include <stdlib.h>

typedef struct {
    void *next;
} mem_block;

typedef struct {
    size_t size;
    mem_block *freelist;
    void *pool;
} mem_pool;


mem_pool *mem_pool_create(size_t size, size_t count) {
    union {
        char data[size];
        #if __SIZEOF_POINTER__ == 8
        long long align;
        #else
        int align;
        #endif
    } aligned_block;

    mem_pool *pool = malloc(sizeof(mem_pool));
    if (pool == NULL) {
        return NULL;
    }

    pool->size = size;
    pool->freelist = NULL;
    pool->pool = malloc(sizeof(aligned_block) * count);
    if (pool->pool == NULL) {
        free(pool);
        return NULL;
    }

    for (size_t i = 0; i < count; i++) {
        mem_block *block = (mem_block *)((char *)pool->pool + (i * sizeof(aligned_block)));
        block->next = pool->freelist;
        pool->freelist = block;
    }

    return pool;
}

void *mem_pool_alloc(mem_pool *pool) {
    if (pool->freelist == NULL) {
        return NULL;
    }

    // Remove a block from the freelist and return it
    mem_block *block = pool->freelist;
    pool->freelist = block->next;
    return block;
}

void mem_pool_free(mem_pool *pool, void *ptr) {
    // Add the block to the freelist
    mem_block *block = (mem_block *)ptr;
    block->next = pool->freelist;
    pool->freelist = block;
}

void mem_pool_destroy(mem_pool *pool) {
    free(pool->pool);
    free(pool);
}
