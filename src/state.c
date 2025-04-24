#include "state.h"

#include <string.h>

void chip8_state_init(bf_state_t* self)
{
    self->i = 0x0000;
    memset(self->memory, 0x00, sizeof(uint8_t) * 0x10000);
    self->aux_arg = NULL;
}
