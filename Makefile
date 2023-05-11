all:
	rm *.o || true
	gcc -c main.c
	gcc -o test.exe main.o -lpthread
	rm *.o
	./test.exe
	rm test.exe