#include "cpu/interpreter.h"
#include "state.h"

#include <assert.h>
#include <auxum/std.h>
#include <stdio.h>

void bf_interpreter_init(bf_interpreter_t* self, bf_state_t* state)
{
    *self = (bf_interpreter_t){0};
    self->running = true;
    self->state = state;
}

void bf_interpreter_step(bf_interpreter_t* self)
{
    if(!self->running) return;
    bf_state_t* const state = self->state;
    bf_instruction_t* current = dynarray_get(self->program, self->pc);
    self->pc++;
    switch(current->op)
    {
    case BF_INSTRUCTION_INC:
        {
            state->memory[self->index]++;
            break;   
        }

    case BF_INSTRUCTION_DEC:
        {
            state->memory[self->index]--;
            break;   
        }

    case BF_INSTRUCTION_NEXT:
        {
            self->index++;
            break;
        }

    case BF_INSTRUCTION_PREV:
        {
            self->index--;
            break;
        }
    
    case BF_INSTRUCTION_JUMP_START:
        {
            if (state->memory[self->index] == 0)
            {
                int depth = 1;
                while (depth > 0 && self->pc < self->program.size)
                {
                    bf_instruction_t* instr = dynarray_get(self->program, self->pc++);
                    if (instr->op == BF_INSTRUCTION_JUMP_START) depth++;
                    else if (instr->op == BF_INSTRUCTION_JUMP_BACK) depth--;
                }
            }
            break;
        }
    
    case BF_INSTRUCTION_JUMP_BACK:
        {
            if (state->memory[self->index] != 0)
            {
                int depth = 1;
                self->pc -= 2;
                while (depth > 0 && self->pc < self->program.size)
                {
                    bf_instruction_t* instr = dynarray_get(self->program, self->pc--);
                    if (instr->op == BF_INSTRUCTION_JUMP_BACK) depth++;
                    else if (instr->op == BF_INSTRUCTION_JUMP_START) depth--;
                }
                self->pc += 2;
            }
            break;
        }
    case BF_INSTRUCTION_INPUT:
        {
            if(state->in != NULL)
                state->memory[self->index] = state->in(state->aux_arg);
            break;
        }

    case BF_INSTRUCTION_OUTPUT:
        {
            if(state->out != NULL)
                state->out(state->aux_arg, state->memory[self->index]);
            break;
        }

    case BF_INSTRUCTION_ADD:
        {
            state->memory[self->index] += (int16_t)current->arg;
            break;
        }

    case BF_INSTRUCTION_MOVE:
        {
            self->index += (int16_t)current->arg;
            break;
        }
        
    case BF_INSTRUCTION_END:
        {
            self->running = false;
            break;
        }
    }
}

size_t bf_interpreter_load_program(bf_interpreter_t* self, char* const rom, bf_optimizations_t optimizations)
{
    self->program = bf_compile_program(rom, optimizations);
    return self->program.size;
}
