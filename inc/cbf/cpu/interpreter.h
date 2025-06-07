#pragma once
#ifndef BF_INTERPTERER_H
#define BF_INTERPTERER_H

#include "../state.h"
#include <stdbool.h>
#include <auxum/data/dynarray.h>

typedef struct bf_interpreter
{
    bool running;
    uint16_t pc;
    bf_state_t* state;
} bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* self, bf_state_t* state);
void bf_interpreter_step(bf_interpreter_t* self);

#endif
