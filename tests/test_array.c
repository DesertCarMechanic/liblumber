

#include <stdio.h>
#include <stdlib.h>

#include <sane.h>

#include "test_common.h"
#include "../src/stack.h"
#include "../src/array.h"

struct Point3D {
    u64 x;
    u64 y;
    u64 z;
};

void print_memory(void *memory, size_t length)
{
    byte8 *m = (byte8 *)memory;

    // struct LStack **stack = l_stack_create();

    for (uint i = 1; i<=length; i++) {
        // l_stack_push_start(stack, );
        printf("%d ", *m);
        if (i % 4 == 0) {
            printf("\n");
        }
        m++;
    }
}

void array_test_append_print_error(void)
{ printf("LArray append operation error"); }
void array_test_append_print_success(void)
{ printf("LArray append operation successful"); }
enum RESULT array_test_append_func(void)
{
    struct LArray *larray = l_array_create(
        sizeof(struct Point3D), 
        sizeof(struct Point3D) // for testing only. normally you would make the block size much larger
    );
    struct Point3D p1;
    p1.x = 10;
    p1.y = 20;
    p1.z = 30;
    struct Point3D p2;
    p2.x = 6;
    p2.y = 9;
    p2.z = 420;

    l_array_append(larray, &p1);
    l_array_append(larray, &p2);

    // printf("Length: %lu\n",larray->length);
    // printf("allocated_blocks: %lu\n",larray->allocated_blocks);
    // printf("memory_block_size: %lu\n",larray->memory_block_size);
    // printf("data_size: %lu\n",larray->data_size);

    assert(larray->length = 2);
    assert(larray->allocated_blocks = 2);
    assert(larray->memory_block_size = sizeof(struct Point3D));
    assert(larray->data_size == sizeof(struct Point3D));

    struct Point3D *point2 = (struct Point3D *)l_array_pop(larray);
    assert(point2->x == 6);
    assert(point2->y == 9);
    assert(point2->z == 420);

    assert(larray->length = 1);
    assert(larray->allocated_blocks = 1);

    struct Point3D *point1 = (struct Point3D *)l_array_pop(larray);
    assert(point1->x == 10);
    assert(point1->y == 20);
    assert(point1->z == 30);

    struct Point3D *empty = (struct Point3D *)l_array_pop(larray);
    assert(empty == NULL);

    assert(larray->length == 0);
    assert(larray->allocated_blocks == 0);

    free(point2);
    free(point1);
    // printf("allocated_blocks: %lu\n",larray->allocated_blocks);

    l_array_destroy(larray);
    return RESULT_SUCCESS;
}

struct MisalignedPerson {
    byte8 first_initial;
    u32 age;
};

void array_test_misaligned_struct_print_error(void)
{ printf("LArray misaligned struct test error"); }
void array_test_misaligned_struct_print_success(void)
{ printf("LArray misaligned struct test successful"); }
enum RESULT array_test_misaligned_struct_func(void)
{
    struct LArray *larray = l_array_create(
        sizeof(struct MisalignedPerson), 
        sizeof(struct MisalignedPerson) // for testing only. normally you would make the block size much larger
    );
    struct MisalignedPerson p1;
    p1.first_initial = 'A';
    p1.age = 56;

    struct MisalignedPerson p2;
    p2.first_initial = 'Z';
    p2.age = 111;

    l_array_append(larray, &p1);
    l_array_append(larray, &p2);

    // printf("Length: %lu\n",larray->length);
    // printf("allocated_blocks: %lu\n",larray->allocated_blocks);
    // printf("memory_block_size: %lu\n",larray->memory_block_size);
    // printf("data_size: %lu\n",larray->data_size);

    // printf("\nsizeof Misaligned Person: %lu\n", sizeof(struct MisalignedPerson));
    // print_memory(larray->data, sizeof(struct MisalignedPerson));

    assert(larray->length == 2);
    assert(larray->allocated_blocks == 2);
    assert(larray->memory_block_size == sizeof(struct MisalignedPerson));
    assert(larray->data_size == sizeof(struct MisalignedPerson));

    struct MisalignedPerson *person2 = (struct MisalignedPerson *)l_array_pop(larray);
    assert(person2->first_initial == 'Z');
    assert(person2->age == 111);

    assert(larray->length == 1);
    assert(larray->allocated_blocks == 1);

    struct MisalignedPerson *person1 = (struct MisalignedPerson *)l_array_pop(larray);
    assert(person1->first_initial == 'A');
    assert(person1->age == 56);

    assert(larray->length == 0);
    assert(larray->allocated_blocks == 0);

    struct MisalignedPerson *empty = (struct MisalignedPerson *)l_array_pop(larray);
    assert(empty == NULL);

    assert(larray->length == 0);
    assert(larray->allocated_blocks == 0);

    free(person1);
    free(person2);
    l_array_destroy(larray);
    return RESULT_SUCCESS;
}


struct TestUnit array_test_append = {
    array_test_append_print_error,
    array_test_append_print_success,
    array_test_append_func,
};

struct TestUnit array_test_misaligned_struct = {
    array_test_misaligned_struct_print_error,
    array_test_misaligned_struct_print_success,
    array_test_misaligned_struct_func,
};






