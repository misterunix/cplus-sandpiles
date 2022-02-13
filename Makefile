CC = g++
CFLAGS = -Wall -O2
LFLAGS = -lm

all: cplus-sandpiles

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp

cplus-sandpiles: main.o 
	$(CC) $(CFLAGS) -o cplus-sandpiles main.o $(LFLAGS)
	