CC = g++
CFLAGS= -std=c++11 -Wall -pedantic -O2 -framework OpenGl -framework GLUT -lobjc

.SUFFIXES: .cc .o

.cc.o:
	$(CC) -c $(CFLAGS) $<

all: main

main: main.o
	$(CC) -o main $(CFLAGS) main.o

main.o: main.cc

clean:
	rm -f *.o
	rm -f *.s
	rm -f main
