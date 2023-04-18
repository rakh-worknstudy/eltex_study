all: mkdir_build server.exe client.exe clean

# -------- Server -------- #
server.exe: server.o eltex_buffer.o
	gcc -o build/server.exe server.o eltex_buffer.o
server.o:
	gcc -c src/server.c
# -------- Client -------- #
client.exe: client.o eltex_buffer.o
	gcc -o build/client.exe client.o eltex_buffer.o
client.o:
	gcc -c src/client.c
# -------- Librrr -------- #
eltex_buffer.o:
	gcc -c src/lib/eltex_buffer.c
# -------- Common -------- #
mkdir_build:
	if [ ! -d "build" ];	\
	then			\
		mkdir build;	\
	fi
clean:
	rm *.o || true
