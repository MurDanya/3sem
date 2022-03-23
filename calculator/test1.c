#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

int fd0, fd1, fd2;

int
check(const char *str_input, const char *str_output, const char *str_outerr, int num_exit_code)
{
    fd0 = open("input1", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd1 = open("output", O_RDWR | O_CREAT | O_TRUNC, 0644);
    fd2 = open("outerr", O_RDWR | O_CREAT | O_TRUNC, 0644);
    char str[100];
    int n = write(fd0, str_input, strlen(str_input));
    lseek(fd0, 0L, SEEK_SET);
    if (fork() == 0) {
        dup2(fd0, 0);
        dup2(fd1, 1);
        dup2(fd2, 2);
        execlp("./solution", "./solution", NULL);
        exit(4);
    }
    int status;
    wait(&status);
    if (WIFEXITED(status) && WEXITSTATUS(status) == num_exit_code) {
        lseek(fd1, 0L, SEEK_SET);
        if ((n = read(fd1, str, 100)) != 0) {
            str[strchr(str, '\n') - str] = '\0';
        } else {
            str[0] = '\0';
        }
        if (!strcmp(str, str_output)) {
            lseek(fd2, 0L, SEEK_SET);
            if ((n = read(fd2, str, 100)) != 0) {
                str[strchr(str, '\n') - str] = '\0';
            } else {
                str[0] = '\0';
            }
            if (!strcmp(str, str_outerr)) {
                close(fd0);
                close(fd1);
                close(fd2);
                return 1;
            }
        }
    }
    close(fd0);
    close(fd1);
    close(fd2);
    return 0;
}

int
main(void)
{
    printf("\n");
    int success = 1;
    if (check("(a + b) * c \n 1 \n 2 \n 3 \n", "9", "", 0)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (simple example)\n");

    if (check("z + y*( 3   +x) \n4\n   5\n   6    \n", "41", "", 0)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check too much spaces)\n");
    if (check("((x + ((z - 3) / y)))\n 1 \n 2 \n 12 \n", "5", "", 0)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (checking parentheses)\n");

    if (check("x+1", "", "Error: End of file, there is no sign of the end of the expression", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check EOF)\n");

    if (check("2(x+3)\n", "", "Error: Missing operation", 1)
            && check("(x / 2) y\n", "", "Error: Missing operation", 1)
            && check("x 1\n", "", "Error: Missing operation", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check missing operation)\n");

    if (check("x+\n", "", "Error: Missing operand", 1)
            && check("-\n", "", "Error: Missing operand", 1)
            && check(" * x + 3\n", "", "Error: Missing operand", 1)
            && check("()\n", "", "Error: Missing operand", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check missing operand)\n");

    if (check("1a\n", "", "Error: Invalid operand", 1)
            && check("a%\n", "", "Error: Invalid operand", 1)
            && check(".\n", "", "Error: Invalid operand", 1)
            && check("^1\n", "", "Error: Invalid operand", 1)
            && check("abcdef1\n", "", "Error: Invalid operand", 1)
            && check("asddefg\n", "", "Error: Invalid operand", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check invalid operand)\n");

    if (check("21474836481234567 + x\n", "", "Error: The constant goes beyond the range of the INT type", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check going beyond the type range)\n");

    if (check("((x + 2)/y\n", "", "Error: No closing parenthesis ')'", 1)
            && check("((x + 2)/y)\n 1 \n 2 \n", "1", "", 0)
            && check("((x + 2)/y))\n", "", "Error: No opening parenthesis '('", 1)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check missing parentheses)\n");

    if (check("(a + b) * c \n -2 \n 2 \n 2 \n", "0", "", 0)
            && check("(a + b) * c \n  \n 2 \n 3 \n", "", "Error: The value of the variable is missing", 2)
            && check("(a + b) * c \n1", "", "Error: End of file, There is no sign of the end of the value", 2)
            && check("x+1 \n123abc\n", "", "Error: Invalid variable value", 2)
            && check("x+1 \n - \n", "", "Error: Invalid variable value", 2)
            && check("x+1 \n . \n", "", "Error: Invalid variable value", 2)
            && check("x+1 \n -. \n", "", "Error: Invalid variable value", 2)
            && check("x+1 \n123456789123456789\n", "", "Error: The value goes beyond the range of the INT type", 2)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check reading values variebles)\n");

    if (check("2147483647 + x \n 1 \n", "", "Error: Overflow during addition", 3)
            && check("y - x \n 1 \n -2147483648 \n", "", "Error: Overflow during subtraction", 3)
            && check("2147483647 * x \n 2 \n", "", "Error: Overflow during multiplication", 3)
            && check("y / x \n -1 \n -2147483648 \n", "", "Error: Overflow during division", 3)
            && check("x + 5 / 0 \n 1 \n", "", "Error: Division by 0", 3)
            && check("5 / x \n 0 \n", "", "Error: Division by 0", 3)) {
        printf("success");
    } else {
        success = 0;
        printf("failure");
    }
    printf(" (check calculation)\n");

    if (success) {
        printf("\nSuccessfully\n\n");
    } else {
        printf("\nFailure!\n");
    }
}