# Makefile for hashmap project
# Daniel Koirala – CSIII

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g
TARGET = testHashmap
SRC = testHashmap.cpp
DEPS = hashmap.h

all: $(TARGET)

$(TARGET): $(SRC) $(DEPS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)

