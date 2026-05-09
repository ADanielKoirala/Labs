CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -g

TARGET   = coffee
OBJS     = main.o customer.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp drink.h customer.h barista.h
	$(CXX) $(CXXFLAGS) -c main.cpp

customer.o: customer.cpp customer.h drink.h
	$(CXX) $(CXXFLAGS) -c customer.cpp

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
