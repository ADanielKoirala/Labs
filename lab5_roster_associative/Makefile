#file to build
sources = rosterMap.cpp

#file to output
target = rosterMap
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