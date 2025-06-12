#ifdef JIT_LIGHTNING

#include <assert.h>
#include <stdio.h>
#include "cbf/state.h"
#include "cbf/cpu/jit_lightning.h"

void test_bf_jit_lightning_init(void)
{
    bf_state_t state = {0};
    uint8_t memory[0x10000] = {0};
    state.memory = memory;

    bf_jit_lightning_t jit = {0};
    bf_jit_lightning_init(&jit, &state);

    assert(jit.state == &state);
    assert(jit.running == true);

    printf("[TEST]: bf_jit_lightning_init passed.\n");
}

void test_bf_jit_lightning_exec_add(void)
{
    char *program = "+++.";
    bf_state_t state = {0};
    uint8_t memory[0x10000] = {0};
    state.memory = memory;

    bf_jit_lightning_t jit = {0};
    bf_jit_lightning_init(&jit, &state);

    size_t size = bf_jit_lightning_load_program(&jit, program, BF_OPTIMIZATIONS_ALL);
    UNUSED(size);
    assert(size > 0);
    assert(jit.running == true);

    while (jit.running)
        bf_jit_lightning_step(&jit);

    // After execution, cell 0 should be 3
    assert(state.memory[0] == 3);

    printf("[TEST]: bf_jit_lightning_exec_add passed.\n");
}

#endif
