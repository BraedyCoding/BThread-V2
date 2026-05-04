#include "Scheduler.hpp"
#include "context_switch.hpp"
#include "Context.hpp"
#include "Bthread.hpp"
#include "memManager.hpp"
#include "commonMacros.hpp"

// Static member definitions
std::vector<BThread*> Scheduler::threads = {};
BThread* Scheduler::current_thread = nullptr;
BThread* Scheduler::previous_thread = nullptr;
Context Scheduler::main_ctx;

// Add thread to scheduler
void Scheduler::addThread(BThread &thread) {
    threads.push_back(&thread);
}

// Yield execution to next thread
void Scheduler::yield() {
    if (threads.size() <= 1) return;

    previous_thread = current_thread;
    current_thread = threads[(current_thread_index() + 1) % threads.size()];

    LOG("[Scheduler] yield() called");
    LOG("[Scheduler] switching from thread %p to %p",
    previous_thread, current_thread);
    context_switch(&previous_thread->ctx, &current_thread->ctx);
}

// Start scheduler
void Scheduler::run() {
    if (threads.empty()) return;

    current_thread = threads[0];

    // First switch: main → first thread
    LOG("[Scheduler] run() starting, switching to first thread");
    context_switch(&main_ctx, &current_thread->ctx);
    LOG("[Scheduler] returned back to main context");

    // When all threads eventually finish and switch back,
    // execution resumes here
}

// Helper to find index of current thread
size_t Scheduler::current_thread_index() {
    for (size_t i = 0; i < threads.size(); ++i) {
        if (threads[i] == current_thread) return i;
    }
    return 0;
}