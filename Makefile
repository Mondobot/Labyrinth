CC=g++
CFLAGS=-O3 -funroll-loops -c -Wall
LDFLAGS=-O2  -lGL -lGLU -lglut
INCLUDEFLAGS=-Iinclude

SOURCES=$(patsubst src/%, %, $(wildcard src/*.cpp))
OBJECTS=$(addprefix bin/, $(addsuffix .o, $(basename ${SOURCES})))

EXECUTABLE=labyrinth

CFLAGS+= -g -DDEBUG

build: $(OBJECTS) $(EXECUTABLE)

run:
	./labyrinth input

$(EXECUTABLE): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $@ $(LDFLAGS)

bin/%.o: src/%.cpp
	$(CC) $(INCLUDEFLAGS) $(CFLAGS) $< -o $@

clean:
	-rm -f ${EXECUTABLE} ${OBJECTS} *.d

.PHONY: all clean

