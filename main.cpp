#include <cstdio>
#include "commonMacros.hpp"
#include "Scheduler.hpp"
#include "Bthread.hpp"

// -----------------------------
// Thread functions
// -----------------------------

void threadA() {
    for (int i = 0; i < 5; i++) {
        LOG("[Thread A] iteration %d", i);
        Scheduler::yield();
    }
    LOG("[Thread A] finished");
}

void threadB() {
    for (int i = 0; i < 5; i++) {
        LOG("[Thread B] iteration %d", i);
        Scheduler::yield();
    }
    LOG("[Thread B] finished");
}

void threadC() {
    for (int i = 0; i < 5; i++) {
        LOG("[Thread C] iteration %d", i);
        Scheduler::yield();
    }
    LOG("[Thread C] finished");
}

// -----------------------------
// main
// -----------------------------

int main() {
    LOG("Starting BThread scheduler test");

    BThread t1(threadA);
    BThread t2(threadB);
    BThread t3(threadC);

    Scheduler::addThread(t1);
    Scheduler::addThread(t2);
    Scheduler::addThread(t3);

    Scheduler::run();

    LOG("Back in main thread (all threads done)");

    exit(EXIT_SUCCESS);
}