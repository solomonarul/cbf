#pragma once
#ifndef BF_INTERPTERER_H
#define BF_INTERPTERER_H

#include "../state.h"
#include <stdbool.h>
#include <stddef.h>
#include <auxum/data/dynarray.h>

typedef struct bf_interpreter
{
    bool running;
    uint16_t pc;
    uint16_t index;
    dynarray_t program;
    bf_state_t* state;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* self, bf_state_t* state);
void bf_interpreter_step(bf_interpreter_t* self);
size_t bf_interpreter_load_program(bf_interpreter_t* self, char* const rom, bf_optimizations_t optimizations);

#endif
