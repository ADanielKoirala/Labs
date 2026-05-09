#file to build
sources = rosterObject.cpp

#file to output
target = rosterObject
compiler = g++
#compiler flags
flags = -std=c++17
#build file
build:
	$(compiler) $(flags) $(sources) -o $(target)

run: build
	./$(target) cs1.txt cs2.txt cs3.txt cs4.txt dropouts.txt

#delete built file
clean:
	rm -f $(target)