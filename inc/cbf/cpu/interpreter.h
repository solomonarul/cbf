#pragma once
#ifndef BF_INTERPTERER_H
#define BF_INTERPTERER_H

#include "../state.h"
#include <stdbool.h>
#include <auxum/data/dynarray.h>

struct bf_interpreter
{
    bool running;
    uint16_t pc;
    dynarray_t instructions; 
    bf_state_t* state;
};
typedef struct bf_interpreter bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* self);
void bf_interpreter_load(bf_interpreter_t* self, const char* program);
void bf_interpreter_step(bf_interpreter_t* self);

#endif
