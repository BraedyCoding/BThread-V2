#define <stdio.h>
#include "commonMacros.hpp"


int main() {
    LOG("This is a log message with no additional arguments.");
    LOG("This is a log message with an integer: %d", 42);
    
    

    //this tells the OS That the process terminated with exit code of 
   // EXIT_SUCCESS / EXIT_FAILURE
    exit(EXIT_SUCCESS);
}