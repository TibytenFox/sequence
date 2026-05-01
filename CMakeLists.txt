cmake_minimum_required(VERSION 3.14)
project(DataStructures LANGUAGES CXX)

# ------------------------------------------------------------
# Header-only library (INTERFACE target)
# ------------------------------------------------------------
add_library(sequence INTERFACE)

# Public include path: all headers are located in the sequence/ folder
target_include_directories(sequence INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Minimum required language standard (C++17 is sufficient for all code)
target_compile_features(sequence INTERFACE cxx_std_17)

# ------------------------------------------------------------
# Executable programs
# ------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_executable(app main.cpp)
    target_link_libraries(app PRIVATE sequence)

    add_executable(run_tests test.cpp)
    target_link_libraries(run_tests PRIVATE sequence)

    enable_testing()
    add_test(NAME DataStructuresTests COMMAND run_tests)
endif()