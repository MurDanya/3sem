FLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm
solution: main.o recursive_descant.o shell.o error_delete.o 
	gcc main.o recursive_descant.o shell.o error_delete.o -o solution $(FLAGS)

main.o: main.c recursive_descant.h shell.h error_delete.h
	gcc main.c -c $(FLAGS)

recursive_descant.o: recursive_descant.c recursive_descant.h error_delete.h
	gcc recursive_descant.c -c $(FLAGS)

shell.o: shell.c recursive_descant.h
	gcc shell.c -c $(FLAGS)

error_delete.o: error_delete.c error_delete.h recursive_descant.h
	gcc error_delete.c -c $(FLAGS)

clean: 
	rm -f *.o solution