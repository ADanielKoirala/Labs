//Daniel Koirala

#include <iostream>
#include <fstream>
#include <string>

class Logger {
    private:
    std::ofstream fout;
    Logger() {
        fout.open("log.txt", std::ios::out | std::ios::app);
        if (!fout.is_open()) {
            std::cerr << "Error opening log file!" << std::endl;
    }
}

Logger (const Logger&) = delete;
Logger& operator=(const Logger&) = delete;
Logger(Logger&&) = delete;
Logger& operator=(Logger&&) = delete;

public:
// Meyers Singleton : created once and destroyed automatically
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void report(const std::string& message) {
        if (fout.is_open()) {
            fout << message << std::endl;
        }
    }

    ~Logger() {
        if (fout.is_open()) {
            fout.flush();
            fout.close();
        }
    }
};

//example for logger

void processA() {
    Logger::getInstance().report("Process A started.");
    // ... processing ...
    Logger::getInstance().report("Process A completed.");
}

void processB() {
    Logger::getInstance().report("Process B started.");
    // ... processing ...
    Logger::getInstance().report("Process B completed.");
}

int main() {
    Logger::getInstance().report("Application started.");
    processA();
    processB();
    Logger::getInstance().report("Application ended.");
    return 0;   
}