CC=clang
CFLAGS=-Wall -g
LDLIBS=-lncurses
OBJECTS=corg.o node.o fileio.o input.o windows.o colors.o util.o help.o
HEADERS=colors.h fileio.h node.h input.h windows.h state.h util.h help.h

all: corg

corg: $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(LDLIBS) -o corg $(OBJECTS) 

clean:
	rm *.o corg
