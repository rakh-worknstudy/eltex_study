all: server.exe client.exe clean


# -------- Server -------- #
server.exe: mkdir_build server.o
	gcc -o build/server.exe server.o
server.o:
	gcc -c src/server.c
# -------- Client -------- #
client.exe: mkdir_build client.o
	gcc -o build/client.exe client.o
client.o:
	gcc -c src/client.c
# -------- Common -------- #
mkdir_build:
	if [ ! -d "build" ];	\
	then			\
		mkdir build;	\
	fi
clean:
	rm *.o || true
