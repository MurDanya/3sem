FLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm -g
VALGRIND=--leak-check=full --show-leak-kinds=all
solution: calculator.c
	gcc calculator.c -o solution $(FLAGS)

test1: test1_sol solution
	./test1_sol

test1_sol: test1.c
	gcc test1.c -o test1_sol

test2: input2 solution
	valgrind $(VALGRIND) ./solution < input2

clean:
	rm -rf *.o solution test1_sol input1 output outerr