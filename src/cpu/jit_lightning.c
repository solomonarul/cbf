#include "cpu/jit_lightning.h"
#include "auxum/data/dynarray.h"
#include "state.h"

#include <auxum/platform/app.h>
#include <auxum/data.h>
#include <lightning.h>

static bool jit_global_init = false;

void bf_jit_lightning_init(bf_jit_lightning_t* self, bf_state_t* state)
{
    self->running = true;
    self->state = state;

    if(!jit_global_init)
    {
        jit_global_init = true;
        init_jit(auxum_get_app_path());
        atexit(finish_jit);
    }
    self->jit_state = jit_new_state();

    dynarray_t loopStartLabel, loopEndLabel;
    dynarray_init(&loopStartLabel, sizeof(jit_pointer_t), 0);
    dynarray_init(&loopEndLabel, sizeof(jit_pointer_t), 0);

    #define _jit self->jit_state

    jit_prolog();
    // jit_movi(JIT_V0, (jit_word_t)self->memory); // V0 is a register pointing to our memory.

    // jit_pointer_t start_jump;

    for(uint32_t index = 0; index < self->state->program.size; index++)
    {
        const bf_instruction_t* instruction = dynarray_get(self->state->program, index);
        switch(instruction->op)
        {
        case BF_INSTRUCTION_INC:
            break;

        case BF_INSTRUCTION_DEC:
            break;

        case BF_INSTRUCTION_NEXT:
            break;
        
        case BF_INSTRUCTION_PREV:
            break;

        case BF_INSTRUCTION_JUMP_START:
            break;

        case BF_INSTRUCTION_JUMP_BACK:
            break;

        case BF_INSTRUCTION_INPUT:
            break;

        case BF_INSTRUCTION_OUTPUT:
            break;

        case BF_INSTRUCTION_END:
            break;

        // Optimized instructions start here.

        case BF_INSTRUCTION_ADD:
            break;
    
        case BF_INSTRUCTION_MOVE:
            break;
        }
    }
    jit_epilog();
    self->code = (bf_jit_function_t)((uintptr_t)jit_emit());
    jit_clear_state();
    dynarray_free(loopStartLabel);
    dynarray_free(loopEndLabel);
    #undef _jit
}

void bf_jit_lightning_step(bf_jit_lightning_t* self)
{
    self->code();
    self->running = false;
}
