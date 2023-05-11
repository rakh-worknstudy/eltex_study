all: build clean
build: main.o window.o list.o
	gcc -o build/eltex_mc.exe main.o window.o list.o -lncurses
main.o:
	gcc -c src/main.c
window.o:
	gcc -c src/window.c
list.o:
	gcc -c src/list.c
clean:
	rm *.o || true
