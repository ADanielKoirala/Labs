#include "processState.hpp"
#include "process.hpp"

// ---- Singleton instances ----
NewState& NewState::instance() {
    static NewState s;
    return s;
}

ReadyState& ReadyState::instance() {
    static ReadyState s;
    return s;
}

RunningState& RunningState::instance() {
    static RunningState s;
    return s;
}

WaitingState& WaitingState::instance() {
    static WaitingState s;
    return s;
}

TerminatedState& TerminatedState::instance() {
    static TerminatedState s;
    return s;
}

// ---- Accessors ----
ProcessState& getNewState()        { return NewState::instance(); }
ProcessState& getReadyState()      { return ReadyState::instance(); }
ProcessState& getRunningState()    { return RunningState::instance(); }
ProcessState& getWaitingState()    { return WaitingState::instance(); }
ProcessState& getTerminatedState() { return TerminatedState::instance(); }

// ---- State names ----
const char* NewState::name() const          { return "new"; }
const char* ReadyState::name() const        { return "ready"; }
const char* RunningState::name() const      { return "running"; }
const char* WaitingState::name() const      { return "waiting"; }
const char* TerminatedState::name() const   { return "terminated"; }

// ---- Transitions ----
void NewState::admitted(Process& p) {
    p.setState(&ReadyState::instance());
    p.report();
}

void ReadyState::schedulerDispatch(Process& p) {
    p.setState(&RunningState::instance());
    p.report();
}

void RunningState::interrupt(Process& p) {
    p.setState(&ReadyState::instance());
    p.report();
}

void RunningState::ioEventWait(Process& p) {
    p.setState(&WaitingState::instance());
    p.report();
}

void RunningState::exit(Process& p) {
    p.setState(&TerminatedState::instance());
    p.report();
}

void WaitingState::ioEventCompletion(Process& p) {
    p.setState(&ReadyState::instance());
    p.report();
}
