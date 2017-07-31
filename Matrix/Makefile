CC = g++
DEBUG = -g -O0 -pedantic-errors
CFLAGS = -c -Wall -std=c++11 $(DEBUG)
LFLAGS =  -Wall $(DEBUG)

all: p1 Matrix.o

p1: p1.o Matrix.o
	$(CC) $(LFLAGS) -o p1 p1.o Matrix.o
p1.o: p1.cpp
	$(CC) $(CFLAGS) p1.cpp
Matrix.o: Matrix.cpp
	$(CC) $(CFLAGS) Matrix.cpp
.PHONY: 
	clean
clean:
	rm -f *.o
	rm -f p1 