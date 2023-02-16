# Using g++ to compile
CC = g++

# Compile flags
#	-wall	- turn on most compiler warnings
#	-g		- add debugging information to the executable
CFLAGS = -Wall -g
ODIR = ./obj
LDIR = ./lib
SDIR = ./src
LIBS = -lSDL2
DEPS = lib/graphics.h

SRC = $(wildcard $(SDIR)/*.cpp)

OBJ = $(patsubst $(SDIR)/%.cpp, $(ODIR)/%.o, $(SRC))



.PHONY: all clean

all: main.out




# $@ matches to the target filename (main.out)
# $< matches to the first dependency name (finaltest.o)
# $^ matches to all dependencies, separated by spaces, duplicated discarded
main.out: $(OBJ) $(DEPS)
	$(CC) $(CFLAGS) -o $@ $(OBJ) -lSDL2

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@



clean:
	rm obj/*
