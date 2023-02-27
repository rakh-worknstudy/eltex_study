all: build clean
build: main.o libcalc.so
	gcc -o prog.exe main.o -L. -lcalc -Wl,-rpath,.
main.o:
	gcc -c main.c
libcalc.so: eltex_calc.o
	gcc -shared -o libcalc.so eltex_calc.o
eltex_calc.o:
	gcc -c eltex_calc.c
clean:
	rm *.o # *.so
