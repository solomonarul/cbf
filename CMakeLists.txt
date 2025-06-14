cmake_minimum_required(VERSION 3.20)

project(cbf)

file(
    GLOB_RECURSE
    ${PROJECT_NAME}_SRC
        "src/state.c"
        "src/cpu/interpreter.c"
)

add_library(
    ${PROJECT_NAME}
    STATIC
        ${${PROJECT_NAME}_SRC}
)

if(ENABLE_JIT_LIGHTNING)
    target_sources(
        ${PROJECT_NAME}
        PRIVATE
            "src/cpu/jit_lightning.c"
    )
    target_link_libraries(
        ${PROJECT_NAME}
        PUBLIC
            lightning
    )
    target_compile_definitions(
        ${PROJECT_NAME}
        PUBLIC
            JIT_LIGHTNING
    )
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
        "inc/"
    PRIVATE
        "inc/cbf"
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC
        auxum
)

set_property(
    TARGET ${PROJECT_NAME}
    PROPERTY C_STANDARD 23
)

if(BUILD_TESTS)
file(
    GLOB_RECURSE
    ${PROJECT_NAME}_TEST_SRC
        "test/main.c"
)

add_executable(
    ${PROJECT_NAME}_test
    ${${PROJECT_NAME}_TEST_SRC}
)

target_compile_options(${PROJECT_NAME}_test PRIVATE -Wall -Wextra -Wpedantic -Werror)

target_link_libraries(
    ${PROJECT_NAME}_test
    PUBLIC
        ${PROJECT_NAME}
)

set_property(
    TARGET ${PROJECT_NAME}_test
    PROPERTY C_STANDARD 23
)
endif()

if(BUILD_BENCHMARKS)
file(
    GLOB_RECURSE
    ${PROJECT_NAME}_BENCH_SRC
        "bench/main.c"
)

add_executable(
    ${PROJECT_NAME}_bench
    ${${PROJECT_NAME}_BENCH_SRC}
)

target_compile_options(${PROJECT_NAME}_bench PRIVATE -Wall -Wextra -Wpedantic -Werror)

if(ENABLE_JIT_LIGHTNING)
    target_compile_definitions(
        ${PROJECT_NAME}_bench
        PUBLIC
            JIT_LIGHTNING
    )
endif()

if(NOT "${SDL3_PATH}" STREQUAL "")
find_package(SDL3 REQUIRED PATHS ${SDL3_PATH})
else()
find_package(SDL3 REQUIRED)
endif()
target_link_libraries(
    ${PROJECT_NAME}_bench
    PUBLIC
        ${PROJECT_NAME}
        SDL3::SDL3           # For accurate timing.
)

set_property(
    TARGET ${PROJECT_NAME}_bench
    PROPERTY C_STANDARD 23
)
endif()

