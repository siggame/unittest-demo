# Author: Kevin Lai <kevinlai31@gmail.com>
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SHARED := $(SOURCES:.cpp=.so)

export LD_LIBRARY_PATH:="$(LD_LIBRARY_PATH):$(PWD)"


all: $(OBJECTS) $(SHARED)

test: $(OBJECTS)
	make -C unittests

main.out: main.o
	g++ $< -o main.out -ldl

%.o: %.cpp $(HEADERS)
	g++ -g -fPIC $(CFLAGS) -c $< -o $@ & \
	g++ -g -fPIC --coverage -c $< -o unittests/$@ & \
	python3 unittests/mutation-candidates.py $< 

%.so : %.o
	g++ -shared -Wl,-soname,lib$@ -o lib/lib$@ $^ -lpthread -lm

clean:
	rm *.o & rm *.gc* & rm *.so & make -C unittests clean
	