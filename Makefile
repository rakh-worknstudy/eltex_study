SRCS = main.c
PROG = test
FLAGS = -fno-stack-protector -no-pie

# ALL
all:     build     run-textest run     rm-prog
all-gdb: build-gdb run-textest run-gdb rm-prog
# BUILD
build:
	gcc $(SRCS) -o $(PROG) $(FLAGS)
build-gdb:
	gcc $(SRCS) -o $(PROG) $(FLAGS) -g
# RUN
run:
	./$(PROG)
run-gdb:
	gdb ./$(PROG)
run-test:
	echo TO DO
# CLEAN
#rm-o:
#	rm *.o
rm-prog:
	rm $(PROG)
# SPECIAL
run-textest:
	gcc -o text text.c
	./text
	rm text
