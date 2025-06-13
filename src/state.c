#include "state.h"
#include "auxum/data/dynarray.h"

#include <string.h>

void bf_state_init(bf_state_t* self)
{
    *self = (bf_state_t){0};
}

void bf_check_hotloops(dynarray_t* result)
{
    typedef struct {
        const bf_instruction_t* instruction_list;
        uint8_t instruction_list_length;
        bf_instruction_t replacement;
    } bf_known_loop_t;

    // 3j xa -1j    
    static const bf_instruction_t CLR_loop[] = {
        {BF_INSTRUCTION_JUMP, 3}, {BF_INSTRUCTION_ADD, 0}, {BF_INSTRUCTION_JUMP, -1}
    };

    // 6j -1a xm 1a xm -4j
    static const bf_instruction_t ADDCLR_loop[] = {
        {BF_INSTRUCTION_JUMP, 6}, {BF_INSTRUCTION_ADD, -1}, {BF_INSTRUCTION_MOVE, 0}, {BF_INSTRUCTION_ADD, 1}, {BF_INSTRUCTION_MOVE, 0}, {BF_INSTRUCTION_JUMP, -4}
    };

    // 3j xm -1j
    static const bf_instruction_t MOVNZ_loop[] = {
        {BF_INSTRUCTION_JUMP, 3}, {BF_INSTRUCTION_MOVE, 0}, {BF_INSTRUCTION_JUMP, -1}
    };

    static uint8_t known_loop_count = 3;

    static const bf_known_loop_t known_loops[] = {
        { CLR_loop, 3, {BF_INSTRUCTION_CLR, 0} },
        { ADDCLR_loop, 6, {BF_INSTRUCTION_ADDCLR, -1} },
        { MOVNZ_loop, 3, {BF_INSTRUCTION_MOVNZ, -1} }
    };

    for(uint32_t index = 0; index < known_loop_count; index++)
    {
        const bf_known_loop_t current_loop = known_loops[index];
        if(result->size < current_loop.instruction_list_length)
            continue;
        uint8_t ok = 1;
        for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length && ok; inner_index++)
        {
            const bf_instruction_t* current_instruction = dynarray_get(*result, result->size - current_loop.instruction_list_length + inner_index);
            if(!(current_loop.instruction_list[inner_index].op == current_instruction->op &&
                ((int16_t)current_loop.instruction_list[inner_index].arg == 0 || (int16_t)current_loop.instruction_list[inner_index].arg == (int16_t)current_instruction->arg)))
                    ok = 0;
        }
        if(ok == 1)
        {
            if((int16_t)current_loop.replacement.arg >= 0)
            {
                for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length; inner_index++)
                    dynarray_pop_back(result);
                dynarray_push_back(result, (void*)&current_loop.replacement);
                return;
            }

            bf_instruction_t replacement_value = current_loop.replacement;
            for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length && ok; inner_index++)
                if((int16_t)current_loop.instruction_list[inner_index].arg == 0)
                {
                    replacement_value.arg++;
                    if((int16_t)replacement_value.arg == 0)
                    {
                        const bf_instruction_t* current_instruction = dynarray_get(*result, result->size - current_loop.instruction_list_length + inner_index);
                        replacement_value.arg = current_instruction->arg;
                        for(uint32_t inner_index = 0; inner_index < current_loop.instruction_list_length; inner_index++)
                            dynarray_pop_back(result);
                        dynarray_push_back(result, (void*)&replacement_value);
                        return;
                    }
                }
        }
    }
}

dynarray_t bf_compile_program(char* const program, bf_optimizations_t optimizations)
{
    dynarray_t result;
    dynarray_init(&result, sizeof(bf_instruction_t), 0);
    int size = strlen(program);
    bf_instruction_t element;
    for(int index = 0; index < size; index++)
        switch(program[index])
        {
        case '+':
            {
                if(optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_INC;
                    element.arg = 0;
                    dynarray_push_back(&result, &element);
                }
                else {
                    if(result.size == 0) goto bf_state_load_program_add_anyway;
                    bf_instruction_t* const last = dynarray_get(result, result.size - 1);
                    if(last->op == BF_INSTRUCTION_ADD)
                    {
                        last->arg = (int16_t)last->arg + 1;
                        if(last->arg == 0)
                            dynarray_pop_back(&result);
                    }
                    else {
                    bf_state_load_program_add_anyway:
                        element.op = BF_INSTRUCTION_ADD;
                        element.arg = 1;
                        dynarray_push_back(&result, &element);
                    }
                }
                break;
            }
        case '-':
            {
                if(optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_DEC;
                    element.arg = 0;
                    dynarray_push_back(&result, &element);
                }
                else {
                    if(result.size == 0) goto bf_state_load_program_remove_anyway;

                    bf_instruction_t* const last = dynarray_get(result, result.size - 1);
                    if(last->op == BF_INSTRUCTION_ADD)
                    {
                        last->arg = (int16_t)last->arg - 1;
                        if(last->arg == 0)
                            dynarray_pop_back(&result);
                    }
                    else {
                    bf_state_load_program_remove_anyway:
                        element.op = BF_INSTRUCTION_ADD;
                        element.arg = (int16_t)-1;
                        dynarray_push_back(&result, &element);
                    }
                }
                break;
            }
        case '>':
            {
                if(optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_NEXT;
                    element.arg = 0;
                    dynarray_push_back(&result, &element);
                }
                else {
                    if(result.size == 0) goto bf_state_load_program_next_anyway;

                    bf_instruction_t* const last = dynarray_get(result, result.size - 1);
                    if(last->op == BF_INSTRUCTION_MOVE)
                    {
                        last->arg = (int16_t)last->arg + 1;
                        if(last->arg == 0)
                            dynarray_pop_back(&result);
                    }
                    else {
                    bf_state_load_program_next_anyway:
                        element.op = BF_INSTRUCTION_MOVE;
                        element.arg = 1;
                        dynarray_push_back(&result, &element);
                    }
                }
                break;
            }
        case '<':
            {
                if(optimizations < BF_OPTIMIZATIONS_INSTRUCTION_FOLDING)
                {
                    element.op = BF_INSTRUCTION_PREV;
                    element.arg = 0;
                    dynarray_push_back(&result, &element);
                }
                else {
                    if(result.size == 0) goto bf_state_load_program_prev_anyway;

                    bf_instruction_t* const last = dynarray_get(result, result.size - 1);
                    if(last->op == BF_INSTRUCTION_MOVE)
                    {
                        last->arg = (int16_t)last->arg - 1;
                        if(last->arg == 0)
                            dynarray_pop_back(&result);
                    }
                    else {
                    bf_state_load_program_prev_anyway:
                        element.op = BF_INSTRUCTION_MOVE;
                        element.arg = (int16_t)-1;
                        dynarray_push_back(&result, &element);
                    }
                }
                break;
            }
        case '[':
            {
                if(optimizations < BF_OPTIMIZATIONS_JUMP_CACHING)
                {
                    element.op = BF_INSTRUCTION_JUMP_START;
                    element.arg = -1;
                    dynarray_push_back(&result, &element);
                }
                else {
                    element.op = BF_INSTRUCTION_JUMP;
                    element.arg = 0;
                    dynarray_push_back(&result, &element);
                }
                break;
            }
        case ']':
            {
                if(optimizations < BF_OPTIMIZATIONS_JUMP_CACHING)
                {
                    element.op = BF_INSTRUCTION_JUMP_BACK;
                    element.arg = -1;
                    dynarray_push_back(&result, &element);
                }
                else {
                    bf_instruction_t* current_instruction;

                    // Find previous open bracket.
                    for(uint32_t index = result.size - 1; index < result.size; index--)
                    {
                        current_instruction = dynarray_get(result, index);
                        
                        // Found it.
                        if(current_instruction->op == BF_INSTRUCTION_JUMP && current_instruction->arg == 0)
                        {
                            if(index == result.size - 1)
                            {
                                dynarray_pop_back(&result);
                                break;
                            }

                            current_instruction->arg = result.size - index + 1;
                            element.op = BF_INSTRUCTION_JUMP;
                            element.arg = index - result.size + 1;
                            dynarray_push_back(&result, &element);
                            break;
                        }

                        if(index == 0)
                        {
                            // TODO: handle unmatched [.
                            break;
                        }
                    }

                    if(optimizations >= BF_OPTIMIZATIONS_LOOP_REPLACEMENT)
                        bf_check_hotloops(&result);
                }
                break;
            }
        case ',':
            {
                element.op = BF_INSTRUCTION_INPUT;
                element.arg = -1;
                dynarray_push_back(&result, &element);
                break;
            }
        case '.':
            {
                element.op = BF_INSTRUCTION_OUTPUT;
                element.arg = -1;
                dynarray_push_back(&result, &element);
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
    dynarray_push_back(&result, &element);
    return result;
}
