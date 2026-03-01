/*
This file is part of Bthread, a user-level threading library.
It will be used to save context of the registers in the stack between context switches
*/

/* IMPORTANT NOTES:
============================================================
x86-64 CPU REGISTER CHEAT SHEET (System V ABI - Linux/macOS)
============================================================

When we context switch a thread, we must save the CPU state.
A CPU state is basically:
    "Where was I executing and what was I doing?"

The MOST important registers we must preserve are the
*callee-saved registers* required by the ABI.

------------------------------------------------------------
CRITICAL REGISTERS FOR THREAD CONTEXT SWITCHING
------------------------------------------------------------

RIP — Instruction Pointer (Program Counter)
-------------------------------------------
• Holds the address of the next instruction to execute.
• This is literally "where the thread is running".
• When we restore RIP, execution resumes exactly there.
• Think: the line of code the CPU is on.

RSP — Stack Pointer
-------------------
• Points to the TOP of the stack (stack grows downward).
• Every function call, local variable, return address lives here.
• If RSP is wrong → instant crash.
• This is the most important register for threads.

RBP — Base Pointer (Frame Pointer)
----------------------------------
• Points to the base of the current stack frame.
• Used for accessing function arguments and local variables.
• Not strictly required for execution, but required by ABI.

------------------------------------------------------------
CALLEE-SAVED GENERAL PURPOSE REGISTERS
(these MUST survive function calls)
------------------------------------------------------------

RBX — General Purpose Register
------------------------------
• Often used to store long-lived values.
• Compiler expects it NOT to change across function calls.

R12, R13, R14, R15 — General Purpose Registers
----------------------------------------------
• Extra registers for long-lived variables.
• The compiler assumes they survive function calls.
• If we don't save them → random bugs and corruption.

------------------------------------------------------------
CALLER-SAVED REGISTERS (we DO NOT save these)
------------------------------------------------------------

RAX — Return value register --> this is where we store our previous RiP 
when we do a function call for example. So lets say PC = 0x02 (next instruction) when we 
evaluate a instruction at 0x01 and we do branch to a function when we want to restore the old PC
so we can return to 0x02 (ie. put PC in MAR and fetch, then put it in the instruction register and execute)
RCX — Argument register
RDX — Argument register
RSI — Argument register
RDI — Argument register
R8  — Argument register
R9  — Argument register
R10, R11 — Temporary registers

Why don't we save these?
Because the ABI says the *caller* must assume they get clobbered.
So the compiler already saves them when needed.

This is why our context switch only saves:
    rbx, rbp, r12, r13, r14, r15, rsp, rip

------------------------------------------------------------
MENTAL MODEL
------------------------------------------------------------

Thread = Stack + Registers

Stack → memory of the thread  
Registers → CPU brain of the thread  

Context switching = swapping brains.

save(old_thread.registers)
load(new_thread.registers)
jump to new RIP

============================================================
*/


//this class will act as our TCB (thread control block)
#pragma once
#include "Common-includes\commonMacros.hpp"

class Context{
    reg_t rbx;
    reg_t rbp;
    reg_t r12;
    reg_t r13;
    reg_t r14;
    reg_t r15;
    reg_t rsp;
    reg_t rip;  
}

// //where I am at in the chatGpt conversation:
// don't we need code to be able to set them? I am just thinking, so a bthread could have an object of context for 
// example and a stack object. When the PC is executing the code on the stack, it could then when the scheduler 
// (which is not made yet, switches the PC to something else), we need to be able to maybe save them somewhere. 
// Not sure if we o save them on the stack as well or in a seperate part of the process. I am asking you because you know 
// better.

//chatgpt basically said that in the bthread instance it has a stack and context object.

//class BThread {
// private:
//     Stack stack;      // memory stack for this thread
//     Context ctx;      // saved CPU context
// };

// When switching threads, you use assembly to:

// Save all callee-saved registers + rsp + rip of the current thread → into its ctx struct

// Load the registers + rsp + rip of the next thread → from its ctx struct into the CPU registers so that 
// execution continues from where it left off on that thread's stack.

//We will have to learn some assembly next!



