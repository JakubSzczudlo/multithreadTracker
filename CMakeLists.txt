cmake_minimum_required(VERSION 2.8.9)
project(multi_thread_tracker)

include(CTest)

include_directories("inc"
"src/os"
"src/analyzer"
"src/queue"
"src/printer"
"src/reader"
"src/utilities"
)
set(CMAKE_C_STANDARD 99)

set(CMAKE_C_COMPILER ${CC})

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Using default GCC C compiler")
    add_compile_options(
        -Wall
        -Wextra
    )
elseif(CMAKE_C_COMPILER STREQUAL "Clang")
    message(STATUS "Using default Clang C compiler")
    add_compile_options(
        -Weverything
    )
endif()

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(multi_thread_tracker ${SOURCES})
target_link_libraries(multi_thread_tracker PRIVATE pthread)



add_library(utilities src/utilities/utilities.c)
add_executable(testModel test/testModel.c)
target_link_libraries(testModel PRIVATE utilities)

add_test(
    NAME MyTest
    COMMAND testModel
)