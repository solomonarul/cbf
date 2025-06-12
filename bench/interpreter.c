#include <SDL3/SDL.h>
#include "cbf/state.h"
#include "cbf/cpu/interpreter.h"

#include <stdio.h>

void bf_interpreter_run_bench(char* const name, char* const code, bf_optimizations_t optimizations)
{
    bf_state_t state = {0};
    uint8_t memory[0x10000] = {0};
    state.memory = memory;
    bf_interpreter_t interpreter = {0};
    bf_interpreter_init(&interpreter, &state);

    size_t program_size = bf_interpreter_load_program(&interpreter, code, optimizations);

    uint64_t begin = SDL_GetPerformanceCounter();

    while(interpreter.running)
        bf_interpreter_step(&interpreter);

    uint64_t end = SDL_GetPerformanceCounter();
    double time_spent = (double)(end - begin) * 1000 / SDL_GetPerformanceFrequency();

    printf("[BNCH]: %s | interpreter | %ld instr. | %lfms  | %ld clocks\n", name, program_size, time_spent, end - begin);
}
