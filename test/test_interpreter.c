#include <assert.h>
#include <stdio.h>
#include <auxum/std.h>
#include "cbf/state.h"
#include "cbf/cpu/interpreter.h"

void test_bf_interpreter_init(void)
{
    bf_state_t state = {0};
    bf_interpreter_t interp = {0};

    bf_interpreter_init(&interp, &state);

    assert(interp.running == true);
    assert(interp.pc == 0);
    assert(interp.index == 0);
    assert(interp.program.size == 0);
    assert(interp.state == &state);

    printf("[TEST]: bf_interpreter_init passed.\n");
}

void test_bf_interpreter_exec_add(void)
{
    bf_state_t state = {0};
    uint8_t memory[0x10000] = {0};
    state.memory = memory;

    bf_interpreter_t interp = {0};
    bf_interpreter_init(&interp, &state);

    char *program = "+++.";
    size_t size = bf_interpreter_load_program(&interp, program, BF_OPTIMIZATIONS_ALL);
    UNUSED(size);

    assert(size > 0);
    assert(interp.running == true);

    while (interp.running)
        bf_interpreter_step(&interp);

    assert(state.memory[0] == 3);

    printf("[TEST]: bf_interpreter_load_and_step passed\n");
}
