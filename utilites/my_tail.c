#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

enum
{
    SIZE_BUF = 1024
};

int fd;

void
close_file(void)
{
    close(fd);
}

void
error(const char *err_messenge, char *name_err_file)
{
    char *str;
    if (name_err_file == NULL) {
        str = malloc((strlen(err_messenge) + 8) * sizeof(*str));
        strcpy(str, "tail: ");
        strcat(str, err_messenge);
        strcat(str, "\n");
        if (write(2, str, strlen(str))) {}
    } else {
        str = malloc((strlen(err_messenge) + strlen(name_err_file) + 10) * sizeof(*str));
        strcpy(str, "tail: ");
        strcat(str, err_messenge);
        strcat(str, "'");
        strcat(str, name_err_file);
        strcat(str, "'");
        strcat(str, "\n");
        if (write(2, str, strlen(str))) {}
    }
    free(str);
    exit(1);
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        error("Missing file operand", NULL);
    }
    char flags = 0;
    if (argv[1][0] == '+' || argv[1][0] == '-') {
        flags = argv[1][0];
    }
    if (flags && argc < 3) {
        error("Missing file operand", NULL);
    }
    if ((flags && argc > 3) || (!flags && argc > 2)) {
        error("Too much arguments", NULL);
    }
    long long num_lines_flag;
    if (flags) {
        char *endptr;
        num_lines_flag = strtoll(argv[1], &endptr, 10);
        if (errno == ERANGE && (num_lines_flag == LLONG_MAX || num_lines_flag == LLONG_MIN)) {
            error("Numerical result out of range", NULL);
        }
        if (flags == '-') {
            if (num_lines_flag == LLONG_MIN) {
                error("Numerical result out of range", NULL);
            }
            num_lines_flag *= -1;
        }
        if (*endptr != '\0') {
            error("Option used in invalid context", NULL);
        }
        argv[1] = argv[2];
    } else {
        flags = '-';
        num_lines_flag = 10;
    }
    struct stat sb;
    if (stat(argv[1], &sb) < 0) {
        error("No such file ", argv[1]);
    }
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        error("Can't open the file ", argv[1]);
    }
    atexit(close_file);
    if (flags == '+') {
        char buf[SIZE_BUF];
        int reading_bytes = 0;
        long long num_reading_lines = 1;
        char *symbol_newline = NULL;
        if (num_lines_flag > 1) {
            while ((reading_bytes = read(fd, buf, SIZE_BUF)) > 0) {
                symbol_newline = buf - 1;
                while ((symbol_newline = memchr(symbol_newline + 1, '\n',
                        reading_bytes - (symbol_newline - buf) - 1)) != NULL) {
                    ++num_reading_lines;
                    if (num_reading_lines == num_lines_flag) {
                        break;
                    }
                }
                if (symbol_newline != NULL) {
                    break;
                }
            }
        }
        if (reading_bytes < 0) {
            error("Can't read to a file ", argv[1]);
        }
        if (symbol_newline != NULL) {
            if (write(1, symbol_newline + 1, reading_bytes - (symbol_newline - buf) - 1) < 0) {
                error("Can't write to output file", NULL);
            }
        }
        while ((reading_bytes = read(fd, buf, SIZE_BUF)) > 0) {
            if (write(1, buf, reading_bytes) < 0) {
                error("Can't write to output file", NULL);
            }
        }
        if (reading_bytes < 0) {
            error("Can't read to a file ", argv[1]);
        }
    }
    if (flags == '-') {
        if (num_lines_flag == 0) {
            exit(0);
        }
        char buf[SIZE_BUF];
        int reading_bytes = 0;
        long long size_file = lseek(fd, 0L, SEEK_END);
        if (size_file > 0) {
            lseek(fd, -1L, SEEK_END);
            if (read(fd, buf, 1) < 0) {
                error("Can't read to a file ", argv[1]);
            }
            if (buf[0] == '\n') {
                --size_file;
                lseek(fd, -1L, SEEK_END);
            }
        }
        long long cur_pos_file = size_file;
        long long num_reading_lines = 0;
        long long size_cur_reading_buf = SIZE_BUF;
        char *symbol_newline = NULL;
        while(cur_pos_file > 0) {
            if (cur_pos_file < SIZE_BUF) {
                size_cur_reading_buf = cur_pos_file;
            }
            lseek(fd, -size_cur_reading_buf, SEEK_CUR);
            cur_pos_file -= size_cur_reading_buf;
            if ((reading_bytes = read(fd, buf, size_cur_reading_buf)) < 0) {
                error("Can't read to a file ", argv[1]);
            }
            symbol_newline = buf - 1;
            while ((symbol_newline = memchr(symbol_newline + 1, '\n',
                    reading_bytes - (symbol_newline - buf) - 1)) != NULL) {
                ++num_reading_lines;
            }
            if (num_reading_lines >= num_lines_flag) {
                break;
            }
            lseek(fd, -reading_bytes, SEEK_CUR);
        }
        if (num_reading_lines >= num_lines_flag) {
            symbol_newline = buf - 1;
            while ((symbol_newline = memchr(symbol_newline + 1, '\n',
                    reading_bytes - (symbol_newline - buf) - 1)) != NULL) {
                --num_reading_lines;
                if (num_reading_lines < num_lines_flag) {
                    break;
                }
            }
            if (write(1, symbol_newline + 1, reading_bytes - (symbol_newline - buf) - 1) < 0) {
                error("Can't write to output file", NULL);

            }
        }
        while ((reading_bytes = read(fd, buf, SIZE_BUF)) > 0) {
            if (write(1, buf, reading_bytes) < 0) {
                error("Can't write to output file", NULL);
            }
        }
        if (reading_bytes < 0) {
            error("Can't read to a file ", argv[1]);
        }
    }
    exit(0);
}