# Default C makefile template
SRC = main.c
BIN = main
OPT = -Wall -Wextra -Wconversion -Wsign-conversion -g \
    -pedantic-errors -Werror
CC  = gcc

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(OPT) -o $@ $^

clean:
	rm $(BIN)
