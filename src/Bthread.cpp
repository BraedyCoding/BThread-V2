#include "includes\Bthread.hpp"
#include "includes\Context.hpp"
#include "includes\context_switch.hpp"


//Start by calling automatically initalizing the stack, 
BThread::BThread(Threadfunc func, void* arg) : stack(), start_func(func), arg(arg) {
    // Initialize the context for this thread
    ctx.rbx = 0;
    ctx.rbp = 0;
    ctx.r12 = 0;
    ctx.r13 = 0;
    

    //Set stacl pointer to the top of the stack 
    //Question: On first context switch, there will not be a previous thread
    //I think we maybe should make a stats thing later on where if the number of threads is 1,
    //then 
    ctx.r14 = (reg_t)stack.getTop(); // Set R14 to the top of the stack
    ctx.r15 = 0;
    ctx.rsp = (reg_t)stack.getTop(); // Set RSP to the top of the stack
    ctx.rip = (reg_t)start_func;      // Set RIP to the start function
}

//problems right now
//1. What do we do about the main thread
//2. what do we do when there is one thread in the round robin scheduler, do we pause the main thread
// and let the other thread run (probably not because then we can't run multiple threads).

