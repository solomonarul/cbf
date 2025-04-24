#include "cpu/interpreter.h"

#include <assert.h>
#include <stdlib.h>

void bf_interpreter_init(bf_interpreter_t* self)
{
    self->state = NULL;
    self->running = false;
}

#define UNUSED(x) (void)(x)

void bf_interpreter_step(bf_interpreter_t* self)
{
    bf_state_t* const state = self->state;
    UNUSED(state);
}
