#include "cpu/interpreter.h"

#include <assert.h>
#include <stdlib.h>
#include <auxum/std.h>

void bf_interpreter_init(bf_interpreter_t* self)
{
    self->state = NULL;
    self->running = false;
}

void bf_interpreter_load(bf_interpreter_t* self, const char* program)
{
    UNUSED(self);
    UNUSED(program);
}

void bf_interpreter_step(bf_interpreter_t* self)
{
    bf_state_t* const state = self->state;
    UNUSED(state);
}
