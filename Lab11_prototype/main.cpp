#include <iostream>
#include <queue>
#include <vector>
#include <random>
#include <memory>

#include "process.hpp"
#include "processState.hpp"

int main() {
    Process prototype;

    std::vector<std::unique_ptr<Process>> allProcs;
    std::queue<Process*> readyQueue;
    std::vector<Process*> waitingSet;

    // Admit 4 processes cloned from prototype
    for (int i = 0; i < 4; i++) {
        auto ptr = std::unique_ptr<Process>(prototype.clone());
        Process* p = ptr.get();

        std::cout << "Admitting ";
        p->report();
        p->admitted();        // New → Ready

        readyQueue.push(p);
        allProcs.push_back(std::move(ptr));
    }

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> actionDist(0, 2);  // 0=exit,1=preempt,2=IO
    std::uniform_real_distribution<double> prob(0.0, 1.0);

    int step = 0;

    while (!readyQueue.empty()) {
        std::cout << "\n---- Scheduler step " << step++ << " ----\n";

        Process* running = readyQueue.front();
        readyQueue.pop();

        if (running->getState() == &ReadyState::instance()) {
            running->schedulerDispatch();  // Ready → Running
        } else {
            running->report();
        }

        if (running->getState() != &RunningState::instance()) {
            continue; // Already terminated somehow
        }

        int action = actionDist(rng);

        if (action == 0) {
            std::cout << "Action: exit\n";
            running->exit(); // Running → Terminated

        } else if (action == 1) {
            std::cout << "Action: preempt\n";
            running->interrupt(); // Running → Ready
            readyQueue.push(running);

        } else {
            std::cout << "Action: I/O request\n";
            running->ioEventWait(); // Running → Waiting
            waitingSet.push_back(running);
        }

        // Handle possible I/O completion
        if (!waitingSet.empty() && prob(rng) < 0.5) {
            size_t idx = rng() % waitingSet.size();
            Process* w = waitingSet[idx];

            std::cout << "I/O completed for process "
                      << w->getPid() << "\n";

            w->ioEventCompletion(); // Waiting → Ready
            readyQueue.push(w);

            waitingSet.erase(waitingSet.begin() + idx);
        }

        // Print summary
        std::cout << "Current states:\n";
        for (auto& up : allProcs)
            up->report();
    }

    std::cout << "\nAll processes terminated or left ready queue.\n";
    return 0;
}
