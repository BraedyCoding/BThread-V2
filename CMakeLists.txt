cmake_minimum_required(VERSION 3.16)
project(BThreads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable assembly
enable_language(ASM)

# IMPORTANT: allow preprocessing in .S files
set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/includes
    ${PROJECT_SOURCE_DIR}/Common-includes
)

# Source files
set(SOURCES
    main.cpp
    src/Bthread.cpp
    src/scheduler.cpp
    src/memManager.cpp
    src/context_switch.S
)

add_executable(bthreads ${SOURCES})

target_compile_options(bthreads PRIVATE -O0 -g -fno-omit-frame-pointer)