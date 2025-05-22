#include "cpu/interpreter.h"
#include "state.h"

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <auxum/std.h>

void bf_interpreter_init(bf_interpreter_t* self)
{
    self->state = NULL;
    self->running = true;
    self->pc = 0;
}

void bf_interpreter_load(bf_interpreter_t* self, const char* program)
{
    UNUSED(self);
    UNUSED(program);
}

void bf_interpreter_step(bf_interpreter_t* self)
{
    bf_state_t* const state = self->state;
    bf_instruction_t* current = dynarray_get(state->program, self->pc);
    self->pc++;
    switch(current->op)
    {
    case BF_INSTRUCTION_INC:
        {
            uint8_t value = self->state->load(self->state->aux_arg, self->state->index);
            self->state->store(self->state->aux_arg, self->state->index, ++value);
            break;   
        }
    case BF_INSTRUCTION_DEC:
        {
            uint8_t value = self->state->load(self->state->aux_arg, self->state->index);
            self->state->store(self->state->aux_arg, self->state->index, --value);
            break;   
        }
    case BF_INSTRUCTION_NEXT:
        {
            self->state->index++;
            break;
        }
    case BF_INSTRUCTION_PREV:
        {
            self->state->index--;
            break;
        }
        case BF_INSTRUCTION_JUMP_START:
        {
            if (state->load(state->aux_arg, state->index) == 0)
            {
                int depth = 1;
                while (depth > 0 && self->pc < state->program.size)
                {
                    bf_instruction_t* instr = dynarray_get(state->program, self->pc++);
                    if (instr->op == BF_INSTRUCTION_JUMP_START) depth++;
                    else if (instr->op == BF_INSTRUCTION_JUMP_BACK) depth--;
                }
            }
            break;
        }
    
    case BF_INSTRUCTION_JUMP_BACK:
        {
            if (state->load(state->aux_arg, state->index) != 0)
            {
                int depth = 1;
                self->pc -= 2;
                while (depth > 0 && self->pc < state->program.size)
                {
                    bf_instruction_t* instr = dynarray_get(state->program, self->pc--);
                    if (instr->op == BF_INSTRUCTION_JUMP_BACK) depth++;
                    else if (instr->op == BF_INSTRUCTION_JUMP_START) depth--;
                }
                self->pc += 2;
            }
            break;
        }
    case BF_INSTRUCTION_INPUT:
        {
            state->store(state->aux_arg, state->index, self->state->in(self->state->aux_arg));
            break;
        }
    case BF_INSTRUCTION_OUTPUT:
        {
            self->state->out(self->state->aux_arg, self->state->load(self->state->aux_arg, self->state->index));
            break;
        }
    case BF_INSTRUCTION_END:
        {
            self->running = false;
            break;
        }
    }
}
