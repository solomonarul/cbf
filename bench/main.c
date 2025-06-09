#include "cbf/state.h"
#include <auxum/std.h>
#ifdef JIT_LIGHTNING
#include "lightning.c"
#endif
#include "interpreter.c"

size_t TEST_COUNT = 2;

char* tests[] = {
    "alphabet.b",
    "mandlebrot.b"
};

char mandlebrot[] = {
    #embed "mandlebrot.b"
};

char alphabet[] = {
    #embed "alphabet.b"
};

char* test_sources[] = {
    alphabet,
    mandlebrot,
};

typedef struct {
    char* test_name;
    char* test_source;
    int optimization_level;
} ThreadArgs;

int run_bench(void* ptr)
{
    ThreadArgs* const args = (ThreadArgs*)ptr;

#ifdef JIT_LIGHTNING
    bf_lightning_run_bench(args->test_name, args->test_source, args->optimization_level);
#endif
    bf_interpreter_run_bench(args->test_name, args->test_source, args->optimization_level);

    return 0;
}

int main(int argc, char* argv[])
{
    UNUSED(argc);
    auxum_set_app_path(argv[0]);

    SDL_Thread* threads[TEST_COUNT];
    ThreadArgs args[TEST_COUNT];

    printf("[BNCH]: Optimization level: INSTRUCTION_FOLDING\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
        args[index].test_name = tests[index];
        args[index].test_source = test_sources[index];
        args[index].optimization_level = BF_OPTIMIZATIONS_INSTRUCTION_FOLDING;
        threads[index] = SDL_CreateThread(run_bench, "BenchThread", &args[index]);
    }
    for (size_t index = 0; index < TEST_COUNT; index++)
        SDL_WaitThread(threads[index], NULL);

    printf("[BNCH]: Optimization level: NONE\n");
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
