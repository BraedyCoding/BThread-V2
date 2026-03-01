#pragma once

#include <stdio.h>

/*
The schuduler will use this to keep track of the state of each thread, 
so it can know which thread is ready to run, which is blocked, etc.
*/
enum State {
    READY = 0,
    RUNNING = 1,
    BLOCKED = 2,
    TERMINATED = 3
};

