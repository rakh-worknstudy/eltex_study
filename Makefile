all: build clean run

build: main.o
	gcc -o prog.exe main.o

main.o:
	gcc -c main.c

clean:
	rm *.o

run:
	valgrind ./prog.exe