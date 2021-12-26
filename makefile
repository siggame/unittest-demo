# Author: Kevin Lai <kevinlai31@gmail.com>
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h) $(wildcard *.hpp)


all: $(OBJECTS)

test: $(OBJECTS)
	make -C unittests

%.o: %.cpp $(HEADERS)
	g++ -g $(CFLAGS) -c $< -o $@ & \
	g++ -g --coverage -c $< -o unittests/$@ & \
	python3 unittests/mutation-candidates.py $<
	

clean:
	rm *.o & rm *.gc* & make -C unittests clean