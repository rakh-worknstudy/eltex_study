all: this.exe clean


# -------- TehWun -------- #
this.exe: mkdir_build main.o
	gcc -o build/this.exe main.o
main.o:
	gcc -c src/main.c
# -------- Common -------- #
mkdir_build:
	if [ ! -d "build" ];	\
	then			\
		mkdir build;	\
	fi
clean:
	rm *.o || true
