#Author: Kevin Lai <zlnh4@umsystem.edu>

# Match all CPP files in this directory (except .test_main.cpp)
SOURCES = $(wildcard *.cpp)

# Take all .cpp files matched and replace .cpp with.o
# So test_name.cpp -> test_name.o
OBJECTS = $(SOURCES:.cpp=.o) $(wildcard *.o)

# Generate coverage report
report: test coverage
	gcovr --html --html-details --root=../ -o coverage/report.html

# Create the coverage directory if not exist
coverage:
	mkdir coverage

# Execute the test and generate coverage data
test: test.out
	bash -c "valgrind --leak-check=summary ./$^; exit 0"
	cd ../ && gcov -mr ../*.cpp
	gcov -mr ../*.hpp

mutate: test.out
	./test.out


# Link objects together. The to-be-tested is compiled from root and placed here
test.out: $(OBJECTS) test_main.o src/mutation.o
	g++ -g --coverage $^ -o $@ -ldl

src/mutation.o: src/mutation.cpp
	g++ -g -c $< -o $@

# Separate rule for test_main. If file in root directory was changed, recompile
# tests except main.
test_main.o: .test_main.cpp
	g++ -g --coverage -c $^ -o $@

clean:
	rm *.o & rm *.out & rm *.gc* & rm coverage/*.html
