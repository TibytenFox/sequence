cmake_minimum_required(VERSION 3.14)
project(my_data_structures VERSION 1.0.0 LANGUAGES CXX)

add_library(my-ds INTERFACE)
target_include_directories(my-ds INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(my-ds INTERFACE cxx_std_20)
add_library(my_ds::my_ds ALIAS my-ds)
