CC=g++
CCLAGS=-c -Wall
LIBFLAGS=-lboost_program_options
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=name

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBFLAGS) -o $@

.cpp.o:
	$(CC) $(CCLAGS) $< -o $@
	
clean:
	rm -rf *.o $(Target)
