CC=clang
CFLAGS=-Wall -g
LDLIBS=-lncurses
OBJECTS=corg.o node.o fileio.o input.o windows.o

all: corg

corg: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDLIBS) -o corg $(OBJECTS) 

clean:
	rm *.o corg
