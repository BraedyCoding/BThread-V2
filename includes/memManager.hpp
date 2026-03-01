#pragma once
#include "Common-includes\commonMacros.hpp"
#include <sys/mman.h>
#include <unistd.h>   // for sysconf (page size)
#include <cerrno>
#include <cstring>
#include <iostream>

class Stack {
private:
    void*  base;       // start of mmap region (guard page)
    size_t size;       // usable stack size (page aligned)
    char*  top;        // initial stack pointer (RSP)
    size_t page_size;  // system page size

public:
    Stack(size_t stack_size = 64 * 1024); //by default, we will make the page 64kb
    ~Stack();

    char* getTop() const;
};
