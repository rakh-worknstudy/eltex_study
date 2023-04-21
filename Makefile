all: clean build clean run-log
build: main.o
	gcc -o build/test.exe main.o -lpthread
main.o:
	gcc -c src/main.c
clean:
	rm *.o || true
run-log:
	./build/test.exe > result.log
	cat result.log