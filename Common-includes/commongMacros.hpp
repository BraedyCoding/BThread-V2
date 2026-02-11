
//These will be the common includes for the project, we will add more as we go along
#include <cstdio>
#include <cstdlib>
#include <string> 



//*******************Common macros for the project*************************//

//determine the architecture of the system (x86-64 or x86-32) using preprocessor directives
#define x86_64 (defined(__x86_64__) || defined(_M_X64) || defined(__amd64__))
#define x86_32 (defined(__i386__) || defined(_M_IX86))


//Log macro (similar to one we use at solace for debugging)
#define LOG(x, ...) { \
    printf("[LOG] %s:%d ", __FILE__, __LINE__); \
    printf(x, ##__VA_ARGS__); \
    printf("\n"); \
}

//Macro to check if a pointer is null
#define CHECK_NULL(ptr) { \
    if (ptr == nullptr){ \
        LOG("Null pointer detected: %s", #ptr); \
        exit(EXIT_FAILURE); \
    } \
}
//common aliases for types
#ifdef __x86_64__
using reg_t = uint64_t;
#else
using reg_t = uint32_t;
#endif



