all: build clean
build: eltex_string.o eltex_stack.o main.o
	gcc -o prog.exe main.o eltex_string.o eltex_stack.o
main.o:
	gcc -c main.c
eltex_string.o:
	gcc -c eltex_string.c
eltex_stack.o:
	gcc -c eltex_stack.c
clean:
	rm *.o
