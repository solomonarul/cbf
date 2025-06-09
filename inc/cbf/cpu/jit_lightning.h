#pragma once
#ifndef BF_JIT_LIGHTNING_H
#define BF_JIT_LIGHTNING_H

#ifndef JIT_LIGHTNING
    #error "JIT file included while JIT is disabled."
#endif

#include "../state.h"

#include <auxum/std.h>
#include <lightning.h>
#include <stdint.h>

typedef void (*bf_jit_function_t)(void);

typedef struct bf_jit_lightning
{
    bool running;
    bf_state_t* state;
    jit_state_t* jit_state;
    uint8_t* memory;
    bf_jit_function_t code;
} bf_jit_lightning_t;

void bf_jit_lightning_init(bf_jit_lightning_t* self, bf_state_t* state);
void bf_jit_lightning_step(bf_jit_lightning_t* self);
void bf_jit_lightning_load_program(bf_jit_lightning_t* self, char* const rom, bf_optimizations_t optimizations);

#endif
