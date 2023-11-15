CC=g++
CCLAGS=-c -Wall #-std=c++17
LIBFLAGS=-lboost_program_options -lcryptopp
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=serv

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) $(LIBFLAGS) -o $@

.cpp.o:
	$(CC) $(CCLAGS) $< -o $@
	
clean:
	rm -rf *.o $(Target)
	rm -rf *.app $(Target)
