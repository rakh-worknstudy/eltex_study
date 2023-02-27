all: build clean
build: main.o libcalc.a
	gcc -o prog.exe main.o -L. -lcalc
main.o:
	gcc -c main.c
libcalc.a: eltex_calc.o
	ar cr libcalc.a eltex_calc.o
eltex_calc.o:
	gcc -c eltex_calc.c
clean:
	rm *.o # *.a 
