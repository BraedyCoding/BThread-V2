#pragma once
//This file was not used, I will just leave it for now though
#include <stdio.h>
#include "CoreTypes.hpp"

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

