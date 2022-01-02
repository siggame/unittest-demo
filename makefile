# Author: Kevin Lai <kevinlai31@gmail.com>
SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SHARED := $(SOURCES:.cpp=.so)
MUT := $(wildcard *.mut)
MUT := $(SOURCES:.cpp=.mut)
MUTDB := $(SOURCES:.cpp=.mutdb)

export LD_LIBRARY_PATH := $(CURDIR)/unittests/lib:$(CURDIR)/unittests/mut:$(LD_LIBRARY_PATH)
export WORKSPACE_FOLDER := $(CURDIR)

all: $(OBJECTS) $(SHARED)
	

test: $(OBJECTS) $(SHARED)
	$(MAKE) -C unittests

run: main.out
	./main.out

main.out: .main.cpp
	g++ $< -o $@ -ldl

# Compile mutated files
mutate: $(MUTDB)
	$(MAKE) -s
	$(MAKE) -j -C ./unittests/mut -s
	$(MAKE) -C ./unittests mutate 

# Using the .mut file, generate .mutdb file after creating the source mutations
%.mutdb : %.mut
	python3 unittests/src/mutator.py $< ./unittests/mut > $@

# Generate the .mut file
%.o: %.cpp $(HEADERS)
	g++ -g -fPIC $(CFLAGS) -c $< -o $@ & \
	g++ -g -fPIC --coverage -c $< -o unittests/$@ & \
	python3 unittests/src/mutation-candidates.py $< 

%.so : %.o
	g++ -shared --coverage -o unittests/lib/$@ unittests/$^ -lgcov -lpthread -lm

clean:
	rm *.o & \
	rm *.gc* & \
	rm *.so & \
	rm *.mut &\
	rm *.mutdb & \
	make -C unittests clean &\
	make -C unittests/mut clean