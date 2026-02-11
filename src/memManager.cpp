#include "Stack.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

// helper: round up to nearest multiple of page size
static size_t page_align(size_t size, size_t page_size)
{
    return ((size + page_size - 1) / page_size) * page_size;
}

Stack::Stack(size_t stack_size)
{
    // 1) get system page size
    page_size = sysconf(_SC_PAGESIZE);

    // 2) round requested stack to full pages
    size = page_align(stack_size, page_size);

    // 3) allocate guard page + usable stack in ONE mmap call
    size_t total_size = size + page_size;

    //base is the logical address of the bottom of the region (which is the part that would be growing downwards in a real stack), and the guard page is at the lowest address of the region. The top of the stack (initial RSP) is at the highest address of the region, which is base + total_size.
    base = mmap(
        nullptr,                       // kernel chooses address
        total_size,                    // guard + stack
        PROT_READ | PROT_WRITE,        // temporary permissions
        MAP_PRIVATE | MAP_ANONYMOUS,   // anonymous memory
        -1,
        0
    );

    if (base == MAP_FAILED) {
        perror("mmap failed");
        std::exit(EXIT_FAILURE);
    }

    // 4) turn first page into guard page
    if (mprotect(base, page_size, PROT_NONE) != 0) {
        perror("mprotect failed");
        std::exit(EXIT_FAILURE);
    }

    // 5) compute initial stack pointer (top of stack) - this stays the same

    //Reason why we have to cast it to a char* from a void *:
    // Pointer arithmetic depends on the type of the pointer. When you do: ptr + 1, you are NOT adding 1 byte.,
    // You are adding sizeof(type pointed to) bytes.
    //we want top to be char* so it points to the address of the top of the stack, and when we want to decrement it 
    //to access the next address, if it was int for example, doing top - 1 would actually move the pointer back by 4 bytes
    // (assuming 4 byte ints), which is not what we want. By using char*, we ensure that pointer arithmetic moves in single byte increments,
    // which is necessary for correctly managing the stack memory.

    top = (char*)base + total_size;

    //ask later why we cast base (void pointer) + a size_t to a char* (I don't understand why we did this)
}

Stack::~Stack()
{
    // free entire region (guard + stack)
    munmap(base, size + page_size);
}

char* Stack::getTop() const
{
    return top;
}
