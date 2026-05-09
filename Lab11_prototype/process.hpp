#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "processState.hpp"
#include <iostream>

class Process {
    static int nextPid;
    int pid;
    ProcessState* state;

public:
    Process();               
    Process(const Process&); 

    Process* clone() const { return new Process(*this); }

    int getPid() const { return pid; }
    ProcessState* getState() const { return state; }
    void setState(ProcessState* s) { state = s; }

    // State delegations
    void admitted()            { state->admitted(*this); }
    void schedulerDispatch()   { state->schedulerDispatch(*this); }
    void interrupt()           { state->interrupt(*this); }
    void ioEventWait()         { state->ioEventWait(*this); }
    void ioEventCompletion()   { state->ioEventCompletion(*this); }
    void exit()                { state->exit(*this); }

    void report() const;
};

#endif
