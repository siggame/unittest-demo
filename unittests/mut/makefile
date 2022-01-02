SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SHARED := $(SOURCES:.cpp=.so)

all: $(SHARED)

%.so: %.cpp
	g++ -I ../../ -shared -o $@ $^ -lpthread -lm

clean:
	rm *.so & rm *.cpp & rm *.o