#ifdef JIT_LIGHTNING

#include <SDL3/SDL.h>
#include "cbf/state.h"
#include "cbf/cpu/jit_lightning.h"

void bf_lightning_run_bench(char* const name, char* const code, bf_optimizations_t optimizations)
{
    bf_state_t state = {0};
    uint8_t memory[0x10000] = {0};
    state.memory = memory;
    bf_jit_lightning_t jit = {0};
    bf_jit_lightning_init(&jit, &state);
    
    size_t program_size = bf_jit_lightning_load_program(&jit, code, optimizations);

    uint64_t begin = SDL_GetPerformanceCounter();

    while(jit.running)
        bf_jit_lightning_step(&jit);

    uint64_t end = SDL_GetPerformanceCounter();
    double time_spent = (double)(end - begin) * 1000 / SDL_GetPerformanceFrequency();

    printf("[BNCH]: %s |         jit | %ld instr. | %lfms  | %ld clocks\n", name, program_size, time_spent, end - begin);
}

#endif
