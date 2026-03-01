#pragma once
#include "Common-includes\commonMacros.hpp"
#include "Context.hpp"
#include "memManager.hpp"

using ThreadFunc = void(*)(void*);

class BThread {
public:
    Stack  stack;
    Context ctx;

    ThreadFunc start_func;
    void* arg;

    bool finished = false;

    //It takes in a function pointer and arguments
    BThread(ThreadFunc func, void* arg);
};
