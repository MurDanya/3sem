#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>

enum
{
    SIZE_BUF = 1024,
    SIZE_NUM = 50
};

void
error(const char *err_messenge, char *name_err_file)
{
    char *str;
    if (name_err_file == NULL) {
        str = malloc((strlen(err_messenge) + 6) * sizeof(*str));
        strcpy(str, "wc: ");
        strcat(str, err_messenge);
        strcat(str, "\n");
        if (write(2, str, strlen(str))) {}
    } else {
        str = malloc((strlen(err_messenge) + strlen(name_err_file) + 8) * sizeof(*str));
        strcpy(str, "wc: ");
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

void
ll_to_str_and_strcat(long long ll_num, char *str_info)
{
    char str_num[SIZE_NUM];
    char digit;
    int index = 0;
    if (ll_num == 0) {
        str_num[index] = '0';
        ++index;
    }
    while (ll_num > 0) {
        digit = ll_num % 10;
        ll_num /= 10;
        str_num[index] = '0' + digit;
        ++index;
    }
    str_num[index] = '\0';
    char tmp;
    for (int i = 0; i < index / 2; ++i) {
        tmp = str_num[i];
        str_num[i] = str_num[index - i - 1];
        str_num[index - i - 1] = tmp;
    }
    strcat(str_info, " ");
    strcat(str_info, str_num);
    return;
}

void
write_information(const char *file_name, long long num_lines, long long num_words,
        long long num_symbols)
{
    char str_info[strlen(file_name) + 3 * SIZE_NUM];
    strcpy(str_info, file_name);
    ll_to_str_and_strcat(num_lines, str_info);
    ll_to_str_and_strcat(num_words, str_info);
    ll_to_str_and_strcat(num_symbols, str_info);
    strcat(str_info, "\n");
    if (write(1, str_info, strlen(str_info)) < 0) {
        error("Can't write to output file", NULL);
        exit(1);
    }
    return;
}

int
main(int argc, char **argv)
{
    if (argc < 2) {
        exit(0);
    }
    long long cur_num_lines;
    long long cur_num_words;
    long long cur_num_symbols;
    int flag_is_word;
    char buf[SIZE_BUF];
    int reading_bytes;
    struct stat sb;
    int fd;
    for (int index = 1; index < argc; ++index) {
        if (stat(argv[index], &sb) < 0) {
            error("No such file ", argv[index]);
        }
        fd = open(argv[index], O_RDONLY);
        if (fd < 0) {
            error("Can't open the file ", argv[index]);
        }
        cur_num_lines = 0;
        cur_num_words = 0;
        cur_num_symbols = 0;
        flag_is_word = 0;
        while ((reading_bytes = read(fd, buf, SIZE_BUF)) > 0) {
            cur_num_symbols += reading_bytes;
            for (int j = 0; j < reading_bytes; ++j) {
                if (isspace(buf[j]) && flag_is_word) {
                    flag_is_word = 0;
                } else if (!isspace(buf[j]) && !flag_is_word) {
                    flag_is_word = 1;
                    ++cur_num_words;
                }
                if (buf[j] == '\n') {
                    ++cur_num_lines;
                }
            }
        }
        close(fd);
        if (reading_bytes < 0) {
            error("Can't read to a file ", argv[index]);
        }
        write_information(argv[index], cur_num_lines, cur_num_words, cur_num_symbols);
    }
    exit(0);
}