#pragma once
#include "commonMacros.hpp"
#include "Context.hpp"
#include "memManager.hpp"
#include "CoreTypes.hpp"

#include <functional>
#include <utility>

// Forward declaration
class BThread;

// Trampoline entry point
void thread_trampoline(BThread* t);

class BThread {
public:
    Stack stack;
    Context ctx;
    std::function<void()> start_func;

    bool finished = false;

    template<typename Func, typename... Args>
    BThread(Func&& f, Args&&... args);

    // Disable copy + move (threads must not be duplicated or moved)
    BThread(const BThread&) = delete;
    BThread& operator=(const BThread&) = delete;
    BThread(BThread&&) = delete;
    BThread& operator=(BThread&&) = delete;
};

template<typename Func, typename... Args>
BThread::BThread(Func&& f, Args&&... args)
    : stack(),
      start_func(std::bind(std::forward<Func>(f),
                           std::forward<Args>(args)...))
{
    // -----------------------------
    // Callee-saved registers only
    // -----------------------------
    ctx.rbx = 0;
    ctx.rbp = 0;
    ctx.r12 = 0;
    ctx.r13 = 0;
    ctx.r14 = 0;
    ctx.r15 = 0;

    // -----------------------------
    // Setup stack (16-byte aligned)
    // -----------------------------
    auto top = reinterpret_cast<reg_t>(
        reinterpret_cast<uintptr_t>(stack.getTop())
    );

    top &= ~0xF;

    ctx.rsp = top;

    // -----------------------------
    // Entry point
    // -----------------------------
    ctx.rip = reinterpret_cast<reg_t>(thread_trampoline);

    ctx.rdi = (reg_t)this;   // ONLY for first entry

    LOG("[BThread] created thread object %p", (void*)this);
    LOG("[BThread] stack top = %p", (void*)stack.getTop());
    LOG("[BThread] entry rip = %p", (void*)thread_trampoline);
    LOG("[BThread] initial rsp = %p", (void*)ctx.rsp);


    


}