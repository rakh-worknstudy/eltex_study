all: build clean
build: server.exe client.exe
server.exe: server.o
	gcc -o build/server.exe server.o
server.o:
	gcc -c src/server.c
client.exe: client.o
	gcc -o build/client.exe client.o
client.o:
	gcc -c src/client.c
clean:
	rm *.o || true
	