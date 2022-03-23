#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "recursive_descant.h"

void
error_shell(const char *name)
{
    char *str = malloc(strlen(name) + 9);
    strcpy(str, "shell: ");
    strcat(str, name);
    perror(str);
    free(str);
}

int
run_node(struct node_cmd *node)
{
    int exit_code = 0;
    if (node->lex ==  DISJUNCTION) {
        exit_code = run_node(node->left_node);
        if (exit_code != 0) {
            exit_code = run_node(node->right_node);
        }
        return exit_code;
    }
    if (node->lex == CONJUNCTION) {
        exit_code = run_node(node->left_node);
        if (exit_code == 0) {
            exit_code = run_node(node->right_node);
        }
        return exit_code;
    }
    if (node->lex == CONVEYOR) {
        pid_t pid;
        int fd[2][2];
        if (pipe(fd[0]) < 0) {
            error_shell("pipe");
            return 1;
        }
        if ((pid = fork()) == 0) {
            dup2(fd[0][0], 0);
            close(fd[0][0]);
            close(fd[0][1]);
            exit_code = run_node(node->right_node);
            exit(exit_code);
        }
        while (node->left_node->lex == CONVEYOR) {
            node = node->left_node;
            if (pipe(fd[1]) < 0) {
                error_shell("pipe");
                return 1;
            }
            if (fork() == 0) {
                dup2(fd[1][0], 0);
                dup2(fd[0][1], 1);
                close(fd[0][0]);
                close(fd[0][1]);
                close(fd[1][0]);
                close(fd[1][1]);
                exit_code = run_node(node->right_node);
                exit(exit_code);
            }
            close(fd[0][0]);
            close(fd[0][1]);
            fd[0][0] = fd[1][0];
            fd[0][1] = fd[1][1];
        }
        if (fork() == 0) {
            dup2(fd[0][1], 1);
            close(fd[0][0]);
            close(fd[0][1]);
            exit_code = run_node(node->left_node);
            exit(exit_code);
        }
        close(fd[0][0]);
        close(fd[0][1]);
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        }
        while (wait(NULL) > 0);
        return exit_code;
    }
    if (node->lex == SEMICOLON) {
        exit_code = run_node(node->left_node);
        if (node->right_node != NULL) {
            exit_code = run_node(node->right_node);
        }
        return exit_code;
    }
    if (node->lex == BACKGROUND_MODE) {
        pid_t pid;
        if ((pid = fork()) == 0) {
            run_node(node->left_node);
            exit(0);
        }
        if (pid < 0) {
            error_shell("fork");
            return 1;
        }
        if (node->right_node != NULL) {
            exit_code = run_node(node->right_node);
        }
        waitpid(pid, NULL, 0);
        return exit_code;
    }
    if (node->lex == REDIRECTION) {
        int fd[2] = {-1, -1};
        if (node->redirection[0] != NULL) {
            fd[0] = open(node->redirection[0], O_RDONLY);
            if (fd[0] == -1) {
                error_shell(node->redirection[0]);
                return 1;
            }
        }
        if (node->redirection[1] != NULL) {
            fd[1] = open(node->redirection[1], O_WRONLY | O_TRUNC | O_CREAT, 0666);
            if (fd[1] == -1) {
                error_shell(node->redirection[1]);
                return 1;
            }
        }
        if (node->redirection[2] != NULL) {
            fd[1] = open(node->redirection[2], O_WRONLY | O_APPEND | O_CREAT, 0666);
            if (fd[1] == -1) {
                error_shell(node->redirection[2]);
                return 1;
            }
        }
        pid_t pid;
        if ((pid = fork()) == 0) {
            if (fd[0] != -1) {
                dup2(fd[0], 0);
                close(fd[0]);
            }
            if (fd[1] != -1) {
                dup2(fd[1], 1);
                close(fd[1]);
            }
            exit_code = run_node(node->left_node);
            exit(exit_code);
        }
        if (pid < 0) {
            error_shell("fork");
            return 1;
        }
        if (fd[0] != -1) {
            close(fd[0]);
        }
        if (fd[1] != -1) {
            close(fd[1]);
        }
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        }
        return exit_code;
    }
    if (node->lex == PROGRAM) {
        pid_t pid;
        if ((pid = fork()) == 0) {
            execvp(node->prog[0], node->prog);
            if (errno == ENOENT) {
                char *str = malloc(strlen(node->prog[0]) + 30);
                strcpy(str, "shell: ");
                strcat(str, node->prog[0]);
                strcat(str, ": command not found");
                fprintf(stderr, "%s\n", str);
                free(str);
            } else {
                error_shell(node->prog[0]);
            }
            exit(127);
        }
        if (pid < 0) {
            error_shell("fork");
            return 1;
        }
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        }
        if (WIFSIGNALED(status)) {
            exit_code = 128 + WTERMSIG(status);
        }
        return exit_code;
    }
    return exit_code;
}

int
run_tree(tree_cmd *tree)
{
    int exit_code = 0;
    if (tree != NULL) {
        exit_code = run_node(tree);
    }
    return exit_code;
}