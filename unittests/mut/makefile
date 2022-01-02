SOURCES := $(wildcard *.cpp)
OBJECTS := $(SOURCES:.cpp=.o)
HEADERS := $(wildcard *.h) $(wildcard *.hpp)
SHARED := $(SOURCES:.cpp=.so)

all: $(OBJECTS) $(SHARED)

%.so: %.o
	g++ -shared -Wl,-soname,$@ -o $@ $^ -lpthread -lm

%.o: %.cpp
	g++ -I ../../ -fPIC $(CFLAGS) -c $< -o $@

clean:
	rm *.so & rm *.cpp & rm *.o