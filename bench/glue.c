#ifdef JIT_LIGHTNING
#include "lightning.c"
#endif
#include "interpreter.c"
#include <SDL3/SDL.h>

typedef struct {
    char* test_name;
    char* test_source;
    int optimization_level;
} ThreadArgs;

int run_lightning_bench(void* ptr)
{
#ifdef JIT_LIGHTNING
    ThreadArgs* args = (ThreadArgs*)ptr;
    bf_lightning_run_bench(args->test_name, args->test_source, args->optimization_level);
#endif
    return 0;
}

int run_interpreter_bench(void* ptr)
{
    ThreadArgs* args = (ThreadArgs*)ptr;
    bf_interpreter_run_bench(args->test_name, args->test_source, args->optimization_level);
    return 0;
}

int run_bench(void* ptr)
{
    ThreadArgs* args = (ThreadArgs*)ptr;

    SDL_Thread* lightning_thread = NULL;
    SDL_Thread* interpreter_thread = NULL;

#ifdef JIT_LIGHTNING
    lightning_thread = SDL_CreateThread(run_lightning_bench, "LightningBench", args);
#endif
    interpreter_thread = SDL_CreateThread(run_interpreter_bench, "InterpreterBench", args);

#ifdef JIT_LIGHTNING
    if (lightning_thread)
        SDL_WaitThread(lightning_thread, NULL);
#endif
    if (interpreter_thread)
        SDL_WaitThread(interpreter_thread, NULL);

    return 0;
}
