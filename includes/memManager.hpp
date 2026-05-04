#pragma once
#include "commonMacros.hpp"
#include <sys/mman.h>
#include <unistd.h>   // for sysconf (page size)
#include <cerrno>
#include <cstring>
#include <iostream>
#include "CoreTypes.hpp"

class Stack {
private:
    void*  base;       // start of mmap region (guard page)
    size_t size;       // usable stack size (page aligned)
    char*  top;        // initial stack pointer (RSP)
    size_t page_size;  // system page size

public:
    Stack(size_t stack_size = PAGE_SIZE * 16); //by default, we will make the stack size 16 pages (128KB on x86-64)
    ~Stack();

    char* getTop() const;
};
