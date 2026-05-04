#include "Bthread.hpp"
#include "Context.hpp"
#include "context_switch.hpp"
#include "memManager.hpp"
#include "commonMacros.hpp"
#include "scheduler.hpp"
#include "CoreTypes.hpp"




void thread_trampoline(BThread* t)
{
    LOG("[trampoline] start %p", t);

    t->start_func();

    LOG("[trampoline] finished %p", t);

    t->finished = true;

    while (true) {
        Scheduler::yield(); // or thread_exit()
    }
}
