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


