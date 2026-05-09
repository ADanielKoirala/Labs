#ifndef PROCESS_STATE_HPP
#define PROCESS_STATE_HPP

#include <iostream>

class Process; 

// Abstract state
class ProcessState {
public:
    virtual ~ProcessState() = default;

    virtual void admitted(Process& p) {}
    virtual void schedulerDispatch(Process& p) {}
    virtual void interrupt(Process& p) {}
    virtual void ioEventWait(Process& p) {}
    virtual void ioEventCompletion(Process& p) {}
    virtual void exit(Process& p) {}

    virtual const char* name() const = 0;
};


class NewState : public ProcessState {
public:
    static NewState& instance();
    const char* name() const override;
    void admitted(Process& p) override;
};

class ReadyState : public ProcessState {
public:
    static ReadyState& instance();
    const char* name() const override;
    void schedulerDispatch(Process& p) override;
};

class RunningState : public ProcessState {
public:
    static RunningState& instance();
    const char* name() const override;
    void interrupt(Process& p) override;
    void ioEventWait(Process& p) override;
    void exit(Process& p) override;
};

class WaitingState : public ProcessState {
public:
    static WaitingState& instance();
    const char* name() const override;
    void ioEventCompletion(Process& p) override;
};

class TerminatedState : public ProcessState {
public:
    static TerminatedState& instance();
    const char* name() const override;
};


ProcessState& getNewState();
ProcessState& getReadyState();
ProcessState& getRunningState();
ProcessState& getWaitingState();
ProcessState& getTerminatedState();

#endif
