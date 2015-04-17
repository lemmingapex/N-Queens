CFLAGS = -O3 -funroll-loops -g -Wall -ansi
#CFLAGS = -O1 -g -Wall -ansi
#CFLAGS = -g -Wall -ansi
CC = g++
SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

SRCS1 = $(wildcard src/backtrack.cpp)
OBJS1 = $(SRCS1:.cpp=.o)
PROG1 = Backtrack

SRCS2 = $(wildcard src/genetic.cpp)
OBJS2 = $(SRCS2:.cpp=.o)
PROG2 = Genetic

SRCS3 = $(wildcard src/minimum-conflicts.cpp)
OBJS3 = $(SRCS3:.cpp=.o)
PROG3 = MinimumConflict

# Windows (cygwin)
ifeq "$(OS)" "Windows_NT"
	PROG1 = Backtrack.exe
	PROG2 = Genetic.exe
	PROG3 = MinimumConflict.exe
endif

all: $(SRCS) $(PROG1) $(PROG2) $(PROG3)

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $(OBJS1) -o $@

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $(OBJS2) -o $@

$(PROG3): $(OBJS3)
	$(CC) $(CFLAGS) $(OBJS3) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -c -o $@

clean:
	rm $(OBJS) $(PROG1) $(PROG2) $(PROG3)
