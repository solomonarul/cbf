#include <auxum/std.h>
#include "test_state.c"
#include "test_interpreter.c"
#include "test_lightning.c"

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    test_bf_state_init();
    test_bf_compile_program();
    test_bf_interpreter_init();
    test_bf_interpreter_exec_add();

#ifdef JIT_LIGHTNING
    test_bf_jit_lightning_init();
    test_bf_jit_lightning_exec_add();
#endif
    return 0;
}
