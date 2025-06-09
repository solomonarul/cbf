#include "cpu/jit_lightning.h"
#include "auxum/data/dynarray.h"
#include "state.h"

#include <auxum/platform/app.h>
#include <auxum/data.h>
#include <lightning.h>
#include <stddef.h>

static bool jit_global_init = false;

void bf_jit_lightning_init(bf_jit_lightning_t* self, bf_state_t* state)
{
    self->running = true;
    self->state = state;
}

void bf_jit_lightning_step(bf_jit_lightning_t* self)
{
    self->code();
    self->running = false;
}

void bf_jit_lightning_load_program(bf_jit_lightning_t* self, char* const rom, bf_optimizations_t optimizations)
{
    dynarray_t program = bf_compile_program(rom, optimizations);

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
    jit_movi(JIT_V0, (jit_word_t)self->memory);

    jit_pointer_t start_jump;

    for(uint32_t index = 0; index < program.size; index++)
    {
        const bf_instruction_t* instruction = dynarray_get(program, index);
        switch(instruction->op)
        {
        case BF_INSTRUCTION_INC:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_addi(JIT_R0, JIT_R0, 1);
            jit_str_c(JIT_V0, JIT_R0);
            break;

        case BF_INSTRUCTION_DEC:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_addi(JIT_R0, JIT_R0, -1);
            jit_str_c(JIT_V0, JIT_R0);
            break;

        case BF_INSTRUCTION_NEXT:
            jit_addi(JIT_V0, JIT_V0, 1);
            break;
        
        case BF_INSTRUCTION_PREV:
            jit_addi(JIT_V0, JIT_V0, -1);
            break;

        case BF_INSTRUCTION_JUMP_START:
        {
            jit_ldr_c(JIT_R0, JIT_V0);

            // Branch if R1 is zero, this will be emitted in the later branch.
            jit_pointer_t end_jump = _jit_new_node_pww(self->jit_state, jit_code_beqi, jit_forward(), JIT_R0, 0);
            dynarray_push_back(&loopEndLabel, &end_jump);

            jit_pointer_t start_label = jit_label();
            dynarray_push_back(&loopStartLabel, &start_label);
            break;
        }

        case BF_INSTRUCTION_JUMP_BACK:
        {
            jit_ldr_c(JIT_R0, JIT_V0);
        
            // Branch if R1 is not zero.
            jit_pointer_t start_label = *(jit_pointer_t*)dynarray_get(loopStartLabel, loopStartLabel.size - 1);
            dynarray_pop_back(&loopStartLabel);

            jit_pointer_t end_label = *(jit_pointer_t*)dynarray_get(loopEndLabel, loopEndLabel.size - 1);
            dynarray_pop_back(&loopEndLabel);

            start_jump = _jit_new_node_pww(self->jit_state, jit_code_bnei, NULL, JIT_R0, 0);
            jit_patch(end_label);
            jit_patch_at(start_jump, start_label);
            break;
        }

        case BF_INSTRUCTION_INPUT:
            jit_prepare();
            jit_pushargi((uintptr_t)(&self));
            jit_finishi((jit_pointer_t)(uintptr_t)self->state->in);
            jit_retval_c(JIT_R0);
            jit_str_c(JIT_V0, JIT_R0);
            break;

        case BF_INSTRUCTION_OUTPUT:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_prepare();
            jit_pushargi((uintptr_t)(&self));
            jit_pushargr_c(JIT_R0);
            jit_finishi((jit_pointer_t)(uintptr_t)self->state->out);
            break;

        case BF_INSTRUCTION_END:
            break;

        // Optimized instructions start here.

        case BF_INSTRUCTION_ADD:
            jit_ldr_c(JIT_R0, JIT_V0);
            jit_addi(JIT_R0, JIT_R0, (int16_t)instruction->arg);
            jit_str_c(JIT_V0, JIT_R0);
            break;
    
        case BF_INSTRUCTION_MOVE:
            jit_addi(JIT_V0, JIT_V0, (int16_t)instruction->arg);
            break;
        }
    }
    jit_epilog();
    self->code = (bf_jit_function_t)((uintptr_t)jit_emit());
    jit_print();
    jit_clear_state();
    dynarray_free(loopStartLabel);
    dynarray_free(loopEndLabel);
    #undef _jit
}
