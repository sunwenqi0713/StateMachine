cmake_minimum_required(VERSION 3.14)
project(state_machine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(state_machine
    src/StateMachine.cpp
    examples/main.cpp
    examples/DemoStates.cpp
)

target_include_directories(state_machine PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/examples"
)

if(MSVC)
  target_compile_options(state_machine PRIVATE /utf-8)
endif()
