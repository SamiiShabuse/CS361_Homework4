CC := g++
CFLAGS := -std=c++17 -Wall -Wextra -pthread 

BIN := bin/race
SRC := src/main.cpp src/race.cpp 

$(BIN): $(SRC)
	@mkdir -p bin
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)

.PHONY: clean doc all

all: $(BIN)

run: all
	$(BIN) 2
	$(BIN) 4
	$(BIN) 10

clean:
	rm -rf bin doc DoxygenOutput

doc:
	doxygen Doxyfile