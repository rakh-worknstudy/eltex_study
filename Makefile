all: build clean
build: main.o window.o
	gcc -o build/eltex_mc.exe main.o window.o -lncurses
main.o:
	gcc -c src/main.c
window.o:
	gcc -c src/window.c
clean:
	rm *.o || true
