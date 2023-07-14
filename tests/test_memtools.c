
#include <stdio.h>
#include <stdlib.h>

#include "test_common.h"
#include "../src/memtools.h"

void memtools_test_copy_blind_error(void)
{ printf("memtools copy blind operation error"); }
void memtools_test_copy_blind_success(void)
{ printf("memtools copy blind operation success"); }
enum RESULT memtools_test_copy_blind_func(void)
{
    return RESULT_SUCCESS;
}

struct TestUnit memtools_test_copy_blind = {
    memtools_test_copy_blind_error,
    memtools_test_copy_blind_success,
    memtools_test_copy_blind_func
};
