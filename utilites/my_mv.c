#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

enum
{
    SIZE_BUF = 1024
};

int fd_source, fd_dest;

void
close_file_source(void)
{
    close(fd_source);
}

void
close_file_dest(void)
{
    close(fd_dest);
}

void
error(const char *err_messenge, char *name_err_file)
{
    if (write(2, "mv: ", 4) < 0 || write(2, err_messenge, strlen(err_messenge)) < 0) {
        exit(2);
    }
    if (name_err_file != NULL) {
        if (write(2, "'", 1) < 0 || write(2, name_err_file, strlen(name_err_file)) < 0
                || write(2, "'", 1) < 0) {
            exit(2);
        }
    }
    if (write(2, "\n", 1) < 0) {
        exit(2);
    }
    exit(1);
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        error("Missing file operand", NULL);
    }
    if (argc < 3) {
        error("Missing destination file operand", NULL);
    }
    if (argc > 3) {
        error("Too much arguments", NULL);
    }
    struct stat sb_source, sb_dest;
    if (stat(argv[1], &sb_source) == 0) {
        if (stat(argv[2], &sb_dest) == 0 && sb_source.st_ino == sb_dest.st_ino) {
            error("The operands are the same file", NULL);
        }
    } else {
        error("No such file ", argv[1]);
    }
    if ((fd_source = open(argv[1], O_RDONLY)) < 0) {
        error("Can't open the file ", argv[1]);
    }
    atexit(close_file_source);
    if ((fd_dest = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
        error("Can't create the file ", argv[2]);
    }
    atexit(close_file_dest);
    char buf[SIZE_BUF];
    int reading_bytes;
    while ((reading_bytes = read(fd_source, buf, SIZE_BUF)) > 0) {
        if (write(fd_dest, buf, reading_bytes) < 0) {
            error("Can't write to a file ", argv[2]);
        }
    }
    if (reading_bytes < 0) {
        error("Can't read to a file ", argv[1]);
    }
    chmod(argv[2], sb_source.st_mode & 0777);
    unlink(argv[1]);
    exit(0);
}