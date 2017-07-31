CC= g++
DEBUG= -g -O0 -pedantic-errors
CFLAGS= -c -Wall -std=c++11 $(DEBUG)
LFLAGS= -Wall $(DEBUG)
COMPILE=$(CC) $(CFLAGS)
LINK= $(CC) $(LFLAGS)


.PHONY: clean 

all: 1730sh command.o

1730sh: 1730sh.o command.o
	$(LINK) -o 1730sh 1730sh.o command.o

1730sh.o: 1730sh.cpp
	$(COMPILE) 1730sh.cpp

command.o: command.cpp
	$(COMPILE) command.cpp

clean: 
	rm -f *.o
	rm -f 1730sh