#include "state.h"

#include <string.h>

void bf_state_init(bf_state_t* self)
{
    self->aux_arg = NULL;
    self->in = NULL;
    self->out = NULL;
    self->store = NULL;
    self->load = NULL;
}
