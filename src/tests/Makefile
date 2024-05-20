run: build
	./testSuite

build: utilTests.o testSuite.o util.o
	gcc -lncurses -o testSuite utilTests.o testSuite.o util.o

testSuite.o:
	gcc -lncurses -o testSuite.c

util.o: ../util.c ../util.h
	gcc -lncurses -c ../util.c

utilTests.o: utilTests.c utilTests.h
	gcc -lncurses -c utilTests.c

clean:
	rm *.o testSuite
