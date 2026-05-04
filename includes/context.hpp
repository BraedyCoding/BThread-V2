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
#include "commonMacros.hpp"
#include "CoreTypes.hpp"


//So in the context_switch.S file, we actually use a offset of either 4 or 8 bytes to access the registers
//so essentially, each register here will actually take up an entire memory word (makes sense because that is how much space a register takes up)
//And that is why we use offset of 4 or 8 bytes in assembly when physicallly writing values to the instance of this strut
struct alignas(16) Context {
    reg_t rbx;
    reg_t rbp;
    reg_t r12;
    reg_t r13;
    reg_t r14;
    reg_t r15;
    reg_t rsp;
    reg_t rip;

    //Argument register (we will only use rdi)
    reg_t rdi;
};

//reg_t is an alias for either uint64_t or uint32_t depending on the architecture (32 vs 64 bit architecture)


//Note: The first field is aligned to the struct’s alignment, 
//which is usually the same as the largest field’s 
//alignment — but not guaranteed by “size”

//so, what we did here is we made the entire context object
//start at a multiple of 16 bytes, that way it works for both 32 bit and 64 bit architectures, because the largest field is either 4 or 8 bytes, so the alignment will be 4 or 8 bytes, and by aligning the entire struct to 16 bytes, we ensure that the first field (rbx) is always at a multiple of 16 bytes, which is what we need for our assembly code to work correctly when we access the fields of this struct.
// and doing rdi + 0*offset  -> rbx for example will still work.

// If we are on a 32-bit architecture, even though reg_t is 4 bytes (not 8),
// the compiler may still insert padding so that the Context object starts
// at a properly aligned memory address (e.g., 16-byte aligned if alignas(16) is used).
// This padding is NOT related to "word size", but rather to alignment requirements
// enforced by the compiler and ABI rules.



//I had to ask AI why 8byte alignment is not enough, and here is the answer it gave me:
// ⚠️ Why 8-byte alignment is NOT enough

// 8-byte alignment ensures:

// 64-bit values are aligned ✔

// BUT it does NOT guarantee:

// correct stack alignment after pushes/calls ❌
// ABI compatibility ❌

// So even though it “matches word size”, it’s incomplete for function execution
//because the ABI requires the stack to be 16-byte aligned at function entry, and if we only align to 8 bytes, we might end up with a stack pointer that is not properly aligned after certain operations (like pushes or calls), which can lead to crashes or undefined behavior when the thread tries to execute code that expects a 16-byte aligned stack. By using alignas(16), we ensure that the entire Context struct is aligned to 16 bytes, which helps maintain proper stack alignment throughout the thread's execution.





