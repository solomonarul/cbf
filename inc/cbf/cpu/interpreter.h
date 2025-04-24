#pragma once
#ifndef BF_INTERPTERER_H
#define BF_INTERPTERER_H

#include "../state.h"
#include <stdbool.h>

struct bf_interpreter
{
    bool running;
    bf_state_t* state;
};
typedef struct bf_interpreter bf_interpreter_t;

void bf_interpreter_init(bf_interpreter_t* self);
void bf_interpreter_step(bf_interpreter_t* self);

#endif
