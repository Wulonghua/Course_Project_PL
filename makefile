CC = g++
CFLAGS = -Wall -c
LFLAGS = -Wall
SOURCES = main.cpp lispparser.cpp lispevaluater.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = FPL

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -f *.o 
