CC = g++
CFLAGS = -Wall -Wextra -g -std=c++1y
PROJECT = test

SRC = main.cpp
OBJ = $(patsubst %.cpp,%.o,$(SRC))

all: $(PROJECT)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $(CFLAGS) $^

%.o: %.cpp
	$(CC) -c $^ $(CFLAGS) -o $@

valgrind:
	valgrind --leak-check=full $(PROJECT)

clean:
	rm $(OBJ) $(PROJECT)
