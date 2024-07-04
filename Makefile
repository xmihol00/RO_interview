# ensure that the Makefile can be called from any directory with 'make -f path/to/Makefile' and still work
MAKEFILE_DIR = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
CURRENT_DIR = $(shell pwd)
PROJECT_DIR = $(MAKEFILE_DIR:$(CURRENT_DIR)/%=%)

CPP_SRC = $(wildcard $(PROJECT_DIR)/src/*.cpp)
CC = $(shell command -v icpx >/dev/null 2>&1 && echo "icpx" || echo "g++") # use intel compiler if available (almost always faster), otherwise use g++
CFLAGS = -Wall -Wextra -std=c++20 -O3

.PHONY: all clean naive

# let's not compile into object files, the program is small enough to compile everything at once
all:
	$(CC) $(CFLAGS) $(CPP_SRC) -o $(PROJECT_DIR)/main

naive:
	$(CC) $(CFLAGS) $(CPP_SRC) -D_NAIVE_APPROACH_ -o $(PROJECT_DIR)/main

clean:
	rm -f $(PROJECT_DIR)/main
