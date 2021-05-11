CC = gcc

# the directories
SRC = src
OBJ = obj
BIN = bin

# the sources and objects
SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

CFLAGS = -c -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=gnu99 -g -I$(SRC)

# the rules and patterns
$(OBJ)/%.o: $(SRC)/%.c
	gcc $(CFLAGS) -o $@ $<

# linking	
all: $(OBJECTS)
	gcc -o $(BIN)/mcc $^
	
clean:
	rm -rf $(OBJ)/*
	rm -rf $(BIN)/*