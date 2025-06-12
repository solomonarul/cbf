#include <assert.h>
#include <stdio.h>
#include "cbf/state.h"

void test_bf_state_init(void)
{
    bf_state_t state = {0};
    bf_state_init(&state);

    assert(state.in == NULL);
    assert(state.out == NULL);
    assert(state.memory == NULL);
    assert(state.aux_arg == NULL);

    printf("[TEST]: bf_state_init passed.\n");
}

void test_bf_compile_program(void)
{
    char *program = "++++";
    dynarray_t compiled = bf_compile_program(program, BF_OPTIMIZATIONS_NONE);

    assert(compiled.size == 5);
    assert(compiled.data != NULL);

    dynarray_free(compiled);

    compiled = bf_compile_program(program, BF_OPTIMIZATIONS_INSTRUCTION_FOLDING);

    assert(compiled.size == 2);
    assert(compiled.data != NULL);

    dynarray_free(compiled);

    printf("[TEST]: bf_compile_program passed.\n");
}
