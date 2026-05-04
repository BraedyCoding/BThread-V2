/*
Context.hpp was a struct that stored registers which each BThread will use,
In this file, it will store the inline assembly code for saving and loading 
registers states, and then putting them in to the context struct.


Idea: When context switching, we will have an old ctx and a new ctx.
So the thread that was just running, we will save its regisers states into the "old ctx",
then we will use the new_ctx and load the register states into the CPU, so that on the next instruction,
it will begin execting the new thread where it left off before.

There will be no C++ implementation of this file, it will be implemented in assembly

*/

#pragma once
#include "Context.hpp"

/*
Switch CPU execution from old_ctx → new_ctx.

This function is implemented in assembly.
It saves current registers into old_ctx
and loads registers from new_ctx.
*/


//So you know labels in assembly, well what C++ does it is adds like a hashed code to each funciton
//instead of it being just the name. So the compiler would do sometihng like b context_switch_1234
// or something like that, and then when we want to call it from assembly, 
///we would have to know the exact name of the function after the compiler mangled it, which is not ideal. 
//By using extern "C", we tell the compiler to not mangle the name of the function, so it will just be context_switch in the assembly code as well, which makes it easier to call from assembly.
extern "C" void context_switch(Context* old_ctx, Context* new_ctx);