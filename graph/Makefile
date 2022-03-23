FLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm
VALGRIND=--leak-check=full --show-leak-kinds=all
solution: main_sol
	./main_sol < input_sol > output

main_sol: main.o graph.o
	gcc main.o graph.o -o main_sol $(FLAGS)

main.o: main.c graph.h
	gcc main.c -c $(FLAGS)

test1: test1_sol
	./test1_sol

test1_sol: test1.o graph.o
	gcc test1.o graph.o -o test1_sol $(FLAGS)

test1.o: test1.c graph.h
	gcc test1.c -c $(FLAGS)

test2: test2_sol
	valgrind $(VALGRIND) ./test2_sol < input2

test2_random: test2_sol
	valgrind $(VALGRIND) ./test2_sol random

test2_sol: test2.o graph.o
	gcc test2.o graph.o -o test2_sol $(FLAGS)

test2.o: test2.c graph.h
	gcc test2.c -c $(FLAGS)

test3: test3_sol
	./test3_sol

test3_sol: test3.o graph.o
	gcc test3.o graph.o -o test3_sol $(FLAGS)

test3.o: test3.c graph.h
	gcc test3.c -c $(FLAGS)

graph.o: graph.c graph.h
	gcc graph.c -c $(FLAGS)

clean: 
	rm -rf *.o main_sol test1_sol test2_sol test3_sol output