#pragma once

#include <cstdio>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <unistd.h>

//******************* Logging *************************//

#define LOG(x, ...) do { \
    printf("[LOG] %s:%d ", __FILE__, __LINE__); \
    printf(x, ##__VA_ARGS__); \
    printf("\n"); \
} while(0)

#define CHECK_NULL(ptr) do { \
    if ((ptr) == nullptr) { \
        LOG("Null pointer detected: %s", #ptr); \
        exit(EXIT_FAILURE); \
    } \
} while(0)



//******************* System info *************************//

inline std::size_t PAGE_SIZE = static_cast<std::size_t>(::sysconf(_SC_PAGESIZE));




