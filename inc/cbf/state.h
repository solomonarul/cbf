#pragma once
#ifndef BF_STATE_H
#define BF_STATE_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t(*bf_input_f)(void*);
typedef void(*bf_output_f)(void*, uint8_t);

struct bf_state
{
    uint16_t i;
    uint8_t memory[0x10000];
    bf_input_f in;
    bf_output_f out;
    void* aux_arg;
};
typedef struct bf_state bf_state_t;

void bf_state_init(bf_state_t* self);

#endif
