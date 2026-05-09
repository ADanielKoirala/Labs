# Makefile for Process State Scheduler

CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
LDFLAGS  = 

# Executable name
TARGET = scheduler

# Object files
OBJS = main.o process.o processState.o

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile rules
main.o: main.cpp process.hpp processState.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

Process.o: process.cpp process.hpp processState.hpp
	$(CXX) $(CXXFLAGS) -c process.cpp

ProcessState.o: processState.cpp processState.hpp process.hpp
	$(CXX) $(CXXFLAGS) -c processState.cpp

# Convenience target to run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)
