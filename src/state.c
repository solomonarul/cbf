#include "state.h"

#include <string.h>

void bf_state_init(bf_state_t* self)
{
    *self = (bf_state_t){0};
}

void bf_state_load_program(bf_state_t* self, char* const program)
{
    dynarray_free(self->program);   // Should do nothing when no program is initialized.
    dynarray_init(&self->program, sizeof(bf_instruction_t), 0);
    int size = strlen(program);
    bf_instruction_t element;
    for(int index = 0; index < size; index++)
        switch(program[index])
        {
        case '+':
            {
                if(self->optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_INC;
                    element.arg = 0;
                    dynarray_push_back(&self->program, &element);
                }
                else {
                    if(self->program.size == 0) goto bf_state_load_program_add_anyway;
                    bf_instruction_t* const last = dynarray_get(self->program, self->program.size - 1);
                    if(last->op == BF_INSTRUCTION_ADD)
                        last->arg = (int16_t)last->arg + 1;
                    else {
                    bf_state_load_program_add_anyway:
                        element.op = BF_INSTRUCTION_ADD;
                        element.arg = 1;
                        dynarray_push_back(&self->program, &element);
                    }
                }
                break;
            }
        case '-':
            {
                if(self->optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_DEC;
                    element.arg = 0;
                    dynarray_push_back(&self->program, &element);
                }
                else {
                    if(self->program.size == 0) goto bf_state_load_program_remove_anyway;

                    bf_instruction_t* const last = dynarray_get(self->program, self->program.size - 1);
                    if(last->op == BF_INSTRUCTION_ADD)
                        last->arg = (int16_t)last->arg - 1;
                    else {
                    bf_state_load_program_remove_anyway:
                        element.op = BF_INSTRUCTION_ADD;
                        element.arg = (int16_t)-1;
                        dynarray_push_back(&self->program, &element);
                    }
                }
                break;
            }
        case '>':
            {
                if(self->optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_NEXT;
                    element.arg = 0;
                    dynarray_push_back(&self->program, &element);
                }
                else {
                    if(self->program.size == 0) goto bf_state_load_program_next_anyway;

                    bf_instruction_t* const last = dynarray_get(self->program, self->program.size - 1);
                    if(last->op == BF_INSTRUCTION_MOVE)
                        last->arg = (int16_t)last->arg + 1;
                    else {
                    bf_state_load_program_next_anyway:
                        element.op = BF_INSTRUCTION_MOVE;
                        element.arg = 1;
                        dynarray_push_back(&self->program, &element);
                    }
                }
                break;
            }
        case '<':
            {
                if(self->optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_PREV;
                    element.arg = 0;
                    dynarray_push_back(&self->program, &element);
                }
                else {
                    if(self->program.size == 0) goto bf_state_load_program_prev_anyway;

                    bf_instruction_t* const last = dynarray_get(self->program, self->program.size - 1);
                    if(last->op == BF_INSTRUCTION_MOVE)
                        last->arg = (int16_t)last->arg - 1;
                    else {
                    bf_state_load_program_prev_anyway:
                        element.op = BF_INSTRUCTION_MOVE;
                        element.arg = (int16_t)-1;
                        dynarray_push_back(&self->program, &element);
                    }
                }
                break;
            }
        case '[':
            {
                element.op = BF_INSTRUCTION_JUMP_START;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case ']':
            {
                element.op = BF_INSTRUCTION_JUMP_BACK;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case ',':
            {
                element.op = BF_INSTRUCTION_INPUT;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case '.':
            {
                element.op = BF_INSTRUCTION_OUTPUT;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        default:
            {
                // Do nothing for unknown characters.
                break;
            }
        }
    element.op = BF_INSTRUCTION_END;
    element.arg = 0;
    dynarray_push_back(&self->program, &element);
}
