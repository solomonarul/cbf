#include "cbf/state.h"
#include <auxum/std.h>
#ifdef JIT_LIGHTNING
#include "lightning.c"
#endif
#include "interpreter.c"

size_t TEST_COUNT = 1;

char* tests[] = {
    "mandlebrot.b"
};

char mandlebrot[] = {
    #embed "mandlebrot.b"
};

char* test_sources[] = {
    mandlebrot
};

int main(int argc, char* argv[])
{
    UNUSED(argc);
    auxum_set_app_path(argv[0]);

    printf("[BNCH]: Optimization level: INSTRUCTION_FOLDING\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
#ifdef JIT_LIGHTNING
        bf_lightning_run_bench(tests[index], test_sources[index], BF_OPTIMIZATIONS_INSTRUCTION_FOLDING);  
#endif 
        bf_interpreter_run_bench(tests[index], test_sources[index], BF_OPTIMIZATIONS_INSTRUCTION_FOLDING);
    }

    printf("[BNCH]: Optimization level: NONE\n");
    for(size_t index = 0; index < TEST_COUNT; index++)
    {
#ifdef JIT_LIGHTNING
        bf_lightning_run_bench(tests[index], test_sources[index], BF_OPTIMIZATIONS_NONE);  
#endif     
        bf_interpreter_run_bench(tests[index], test_sources[index], BF_OPTIMIZATIONS_NONE);
    }

    return 0;
}
