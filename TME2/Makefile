CC = g++
CFLAGS = -Wall -Wextra -g -std=c++1y
PROJECT = tme2

SRC = main.cpp

INCLUDE = hashmap.hpp
OBJ = $(patsubst %.cpp,%.o,$(SRC))

all: $(PROJECT)

$(PROJECT): $(OBJ) $(INCLUDE)
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.cpp
	$(CC) -c $^ $(CFLAGS) -o $@

valgrind:
	valgrind --leak-check=full $(PROJECT)

clean:
	rm $(OBJ)
