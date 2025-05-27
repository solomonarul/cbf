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
                element.op = BF_INSTRUCTION_INC;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case '-':
            {
                element.op = BF_INSTRUCTION_DEC;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case '>':
            {
                element.op = BF_INSTRUCTION_NEXT;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
                break;
            }
        case '<':
            {
                element.op = BF_INSTRUCTION_PREV;
                element.arg = 0;
                dynarray_push_back(&self->program, &element);
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
