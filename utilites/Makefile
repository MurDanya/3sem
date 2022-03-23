FLAGS=-O2 -ftrapv -fsanitize=undefined -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wstrict-prototypes -Wold-style-declaration -Wold-style-definition -Wmissing-parameter-type -Wmissing-field-initializers -Wnested-externs -Wno-pointer-sign -Wcast-qual -Wwrite-strings -std=gnu11 -lm -g

mv: my_mv.c
	gcc my_mv.c -o my_mv $(FLAGS)

wc: my_wc.c
	gcc my_wc.c -o my_wc $(FLAGS)

tail: my_tail.c
	gcc my_tail.c -o my_tail $(FLAGS)

clean:
	rm -rf my_mv my_wc my_tail