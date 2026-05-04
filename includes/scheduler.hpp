#pragma once
#include <vector>
#include "Context.hpp"
#include "CoreTypes.hpp"

class BThread;  // forward declaration

class Scheduler {
public:
    static void addThread(BThread &thread);
    static void yield();
    static void run();
    static size_t current_thread_index();

private:
    static std::vector<BThread*> threads;
    static BThread* current_thread;
    static BThread* previous_thread;
    static Context main_ctx;
};