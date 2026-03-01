//Note: This stack only gets used and "grows downward" when teh R14 stack regsister in the CPU 
//points to the top of the stack. when we push things to the stack, the cpu naturally decrements the stack pointer
//stored in R14, so we don't actually have to worry about handling the decrementation manually.

#include "context.hpp"

#include <sys/mman.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>

// helper: round up to nearest multiple of page size
static size_t page_align(size_t size)
{
    return ((size + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

Stack::Stack(size_t stack_size)
{
    // 1) get system page size
    page_size = sysconf(_SC_PAGESIZE);

    // 2) round requested stack to full pages
    size = page_align(stack_size);
    // 3) allocate guard page + usable stack in ONE mmap call
    size_t total_size = size + page_size; //we do size + page_size so we can add the guard page

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
