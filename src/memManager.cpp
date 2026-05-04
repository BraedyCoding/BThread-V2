#include "memManager.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include "CoreTypes.hpp"

static size_t page_align(size_t size)
{
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

Stack::Stack(size_t stack_size)
{
    page_size = ::sysconf(_SC_PAGESIZE);

    size = page_align(stack_size);
    size_t total_size = size + page_size;

    std::cout << "[Stack] requested size = " << stack_size << "\n";
    std::cout << "[Stack] page_size = " << page_size << "\n";
    std::cout << "[Stack] aligned size = " << size << "\n";
    std::cout << "[Stack] total mmap size = " << total_size << "\n";

    base = mmap(
        nullptr,
        total_size,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (base == MAP_FAILED) {
        perror("mmap failed");
        std::exit(EXIT_FAILURE);
    }

    std::cout << "[Stack] base address = " << base << "\n";

    // guard page
    if (mprotect(base, page_size, PROT_NONE) != 0) {
        perror("mprotect failed");
        std::exit(EXIT_FAILURE);
    }

    std::cout << "[Stack] guard page set at base\n";

    // ----------------------------------------
    // STACK SETUP (CORRECT)
    // ----------------------------------------

    char* stack_bottom = static_cast<char*>(base) + page_size;
    char* stack_top    = stack_bottom + size;

    std::cout << "[Stack] bottom = " << (void*)stack_bottom << "\n";
    std::cout << "[Stack] raw top = " << (void*)stack_top << "\n";

    // align to 16-byte boundary (ABI requirement)
    stack_top = reinterpret_cast<char*>(
        reinterpret_cast<uintptr_t>(stack_top) & ~static_cast<uintptr_t>(0xF)
    );

    std::cout << "[Stack] aligned top (RSP) = " << (void*)stack_top << "\n";

    if ((reinterpret_cast<uintptr_t>(stack_top) % 16) == 0) {
        std::cout << "[Stack] alignment OK ✔\n";
    } else {
        std::cout << "[Stack] alignment BAD ❌\n";
    }

    top = stack_top;
}

Stack::~Stack()
{
    munmap(base, size + page_size);
}

char* Stack::getTop() const
{
    return top;
}