#pragma once
#ifndef BF_STATE_H
#define BF_STATE_H

#include <stdbool.h>
#include <stdint.h>
#include <auxum/data.h>

typedef uint8_t(*bf_input_f)(void*);
typedef void(*bf_output_f)(void*, uint8_t);
typedef void(*bf_store_f)(void*, uint16_t, uint8_t);
typedef uint8_t(*bf_load_f)(void*, uint16_t);

typedef enum bf_operation
{
    // Basic instructions, no optimizations
    BF_INSTRUCTION_INC = 0,
    BF_INSTRUCTION_DEC,
    BF_INSTRUCTION_NEXT,
    BF_INSTRUCTION_PREV,
    BF_INSTRUCTION_JUMP_START,
    BF_INSTRUCTION_JUMP_BACK,
    BF_INSTRUCTION_INPUT,
    BF_INSTRUCTION_OUTPUT,
    BF_INSTRUCTION_END,

    // Optimized instructions
    BF_INSTRUCTION_ADD,
    BF_INSTRUCTION_MOVE
} bf_operation_t;

typedef struct bf_instruction
{
    bf_operation_t op;
    uint16_t arg;
} bf_instruction_t;

typedef enum bf_optimizations {
    BF_OPTIMIZATIONS_NONE = 0,
    BF_OPTIMIZATIONS_INSTRUCTION_FOLDING,
    BF_OPTIMIZATIONS_ALL
} bf_optimizations_t;

typedef struct bf_state
{
    bf_input_f in;
    bf_output_f out;
    uint8_t* memory;
    void* aux_arg;
} bf_state_t;

void bf_state_init(bf_state_t* self);
dynarray_t bf_compile_program(char* const program, bf_optimizations_t optimizations);

#endif
