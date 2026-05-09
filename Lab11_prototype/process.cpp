#include "process.hpp"

int Process::nextPid = 1;

Process::Process()
    : pid(nextPid++), state(&getNewState()) {}

Process::Process(const Process&)
    : pid(nextPid++), state(&getNewState()) {}

void Process::report() const {
    std::cout << "process " << pid
              << " is " << state->name() << "\n";
}
