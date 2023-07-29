CC=gcc
CFLAGS=-Wall
LDLIBS=-lncurses
OBJECTS=corg.o node.o

all: corg

corg: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) -o corg $(OBJECTS) 

clean:
	rm *.o corg