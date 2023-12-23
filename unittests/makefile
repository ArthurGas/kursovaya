.PHONY: all clean test format

CXX=g++
OFLAGS=-O2 -Wall -DNDEBUG
LDFLAGS=-lUnitTest++ -lboost_program_options -lcryptopp

SOURCES=$(wildcard *.cpp)
HEADERS=$(wildcard *.h)
EXECUTABLE=unittest


$(EXECUTABLE): $(SOURCES)
	$(CXX)  $(SOURCES)  -o $(EXECUTABLE) $(OFLAGS) $(LDFLAGS)

test: $(EXECUTABLE)
	./$(EXECUTABLE) 

format:
	astyle $(SOURCES) $(HEADERS)
clean:
	rm -f $(EXECUTABLE) *.orig
