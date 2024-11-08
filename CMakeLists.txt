
cmake_minimum_required(VERSION 3.23)
project(mouse-control-test)

enable_testing()
add_subdirectory(lib)

add_executable(
    mouse-control-test
    src/main.cpp
    )

target_compile_features(
    mouse-control-test
    PRIVATE
    cxx_std_17
    )

find_package(Threads)
target_link_libraries(
    mouse-control-test
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    -lSDL2
    )

add_subdirectory(test)

