#include <auxum/std.h>
#include "cbf/state.h"
#include "glue.c"

size_t TEST_COUNT = 5;

char* tests[] = {
    "alphabet.b",
    "hanoi.b",
    "sierpinski.b",
    "mandlebrot.b",
    "squares.b"
};

char hanoi[] = {
    #embed "hanoi.b"
};

char mandlebrot[] = {
    #embed "mandlebrot.b"
};

char sierpinski[] = {
    #embed "sierpinski.b"
};

char alphabet[] = {
    #embed "alphabet.b"
};

char squares[] = {
    #embed "squares.b"
};

char* test_sources[] = {
    alphabet,
    hanoi,
    sierpinski,
    mandlebrot,
    squares
};

int main(int argc, char* argv[])
{
    UNUSED(argc);
    auxum_set_app_path(argv[0]);

    SDL_Thread* threads[TEST_COUNT];
    ThreadArgs args[TEST_COUNT];

    printf("\n[BNCH]: Optimization level: LOOP_REPLACEMENT\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
        args[index].test_name = tests[index];
        args[index].test_source = test_sources[index];
        args[index].optimization_level = BF_OPTIMIZATIONS_LOOP_REPLACEMENT;
        threads[index] = SDL_CreateThread(run_bench, "BenchThread", &args[index]);
    }
    for (size_t index = 0; index < TEST_COUNT; index++)
        SDL_WaitThread(threads[index], NULL);

    printf("\n[BNCH]: Optimization level: INSTRUCTION_FOLDING\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
        args[index].test_name = tests[index];
        args[index].test_source = test_sources[index];
        args[index].optimization_level = BF_OPTIMIZATIONS_INSTRUCTION_FOLDING;
        threads[index] = SDL_CreateThread(run_bench, "BenchThread", &args[index]);
    }
    for (size_t index = 0; index < TEST_COUNT; index++)
        SDL_WaitThread(threads[index], NULL);

    printf("\n[BNCH]: Optimization level: JUMP_CACHING\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
        args[index].test_name = tests[index];
        args[index].test_source = test_sources[index];
        args[index].optimization_level = BF_OPTIMIZATIONS_JUMP_CACHING;
        threads[index] = SDL_CreateThread(run_bench, "BenchThread", &args[index]);
    }
    for (size_t index = 0; index < TEST_COUNT; index++)
        SDL_WaitThread(threads[index], NULL);

    printf("\n[BNCH]: Optimization level: NONE\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
        args[index].test_name = tests[index];
        args[index].test_source = test_sources[index];
        args[index].optimization_level = BF_OPTIMIZATIONS_NONE;
        threads[index] = SDL_CreateThread(run_bench, "BenchThread", &args[index]);
    }
    for (size_t index = 0; index < TEST_COUNT; index++)
        SDL_WaitThread(threads[index], NULL);

    return 0;
}
