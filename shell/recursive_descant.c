#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "recursive_descant.h"
#include "error_delete.h"

struct node_cmd * parse_separate(int *curr_sym);

void
word_extention(int necessary_size, int *curr_size, char **string)
{
    if (necessary_size > *curr_size) {
        *curr_size = (*curr_size == 0) ? (1) : (*curr_size * 2);
        *string = realloc(*string, *curr_size * sizeof(**string));
    }
    return;
}

void
prog_extention(int necessary_size, int *curr_size, char ***string)
{
    if (necessary_size > *curr_size) {
        *curr_size = (*curr_size == 0) ? (1) : (*curr_size * 2);
        *string = realloc(*string, *curr_size * sizeof(**string));
    }
    return;
}

int
is_operation(int sym)
{
    if (sym == ';' || sym == '&' || sym == '|' || sym == '<' || sym == '>' || sym == '(' || sym == ')') {
        return 1;
    }
    return 0;
}

char
get_next_sym(void)
{
    int sym;
    do {
        sym = getchar();
    } while (isspace(sym));
    return sym;
}

char *
get_filename(int *curr_sym)
{
    if (isspace(*curr_sym)) {
        *curr_sym = get_next_sym();
    }
    char *filename = NULL;
    int size_filename = 0;
    int index_filename = 0;
    while (!is_operation(*curr_sym) && !isspace(*curr_sym) && *curr_sym != EOF) {
        word_extention(index_filename + 1, &size_filename, &filename);
        filename[index_filename] = *curr_sym;
        ++index_filename;
        *curr_sym = getchar();
    }
    if (index_filename == 0) {
        return NULL;
    }
    word_extention(index_filename + 1, &size_filename, &filename);
    filename[index_filename] = '\0';
    if (isspace(*curr_sym)) {
        *curr_sym = get_next_sym();
    }
    return filename;
}

struct node_cmd *
parse_prog(int *curr_sym)
{
    struct node_cmd *ans;
    if (*curr_sym == '(') {
        *curr_sym = get_next_sym();
        ans = parse_separate(curr_sym);
        if (*curr_sym != ')') {
            error(NO_CLOSE_PR, SET_ERROR, ans);
            return NULL;
        }
        *curr_sym = get_next_sym();
    } else if (*curr_sym == EOF) {
        return NULL;
    } else {
        if (is_operation(*curr_sym)) {
            error(MISSING_OPERAND, SET_ERROR, NULL);
            return NULL;
        }
        ans = malloc(sizeof(struct node_cmd));
        ans->lex = PROGRAM;
        ans->left_node = NULL;
        ans->right_node = NULL;
        ans->prog = NULL;
        char *word;
        int size_word, index_word;
        int size_prog = 0;
        int index_prog = 0;
        while (!is_operation(*curr_sym) && *curr_sym != EOF) {
            word = NULL;
            size_word = 0;
            index_word = 0;
            while (!is_operation(*curr_sym) && !isspace(*curr_sym) && *curr_sym != EOF) {
                word_extention(index_word + 1, &size_word, &word);
                word[index_word] = *curr_sym;
                ++index_word;
                *curr_sym = getchar();
            }
            word_extention(index_word + 1, &size_word, &word);
            word[index_word] = '\0';
            prog_extention(index_prog + 1, &size_prog, &ans->prog);
            ans->prog[index_prog] = word;
            ++index_prog;
            if (isspace(*curr_sym)) {
                *curr_sym = get_next_sym();
            }
        }
        if (index_prog == 0) {
            free(ans);
            return NULL;
        }
        prog_extention(index_prog + 1, &size_prog, &ans->prog);
        ans->prog[index_prog] = NULL;
    }
    return ans;
}

struct node_cmd *
parse_redir(int *curr_sym)
{
    struct node_cmd *left, *ans;
    int operation;
    left = parse_prog(curr_sym);
    if (left == NULL) {
        return NULL;
    }
    if (*curr_sym == '>' || *curr_sym == '<') {
        ans = malloc(sizeof(struct node_cmd));
        ans->left_node = left;
        ans->right_node = NULL;
        ans->lex = REDIRECTION;
        ans->redirection = malloc(3 * sizeof(*(ans->redirection)));
        ans->redirection[0] = NULL;
        ans->redirection[1] = NULL;
        ans->redirection[2] = NULL;
        while (*curr_sym == '>' || *curr_sym == '<') {
            operation = *curr_sym;
            *curr_sym = getchar();
            if (operation == '<') {
                if ((ans->redirection[0] = get_filename(curr_sym)) == NULL) {
                    error(NO_FILENAME, SET_ERROR, ans);
                    return NULL;
                }
            }
            if (operation == '>' && *curr_sym != '>') {
                if ((ans->redirection[1] = get_filename(curr_sym)) == NULL) {
                    error(NO_FILENAME, SET_ERROR, ans);
                    return NULL;
                }
                ans->redirection[2] = NULL;
            }
            if (operation == '>' && *curr_sym == '>') {
                *curr_sym = getchar();
                if ((ans->redirection[2] = get_filename(curr_sym)) == NULL) {
                    error(NO_FILENAME, SET_ERROR, ans);
                    return NULL;
                }
                ans->redirection[1] = NULL;
            }
        }
        left = ans;
    }
    return left;
}

struct node_cmd *
parse_conveyor(int *curr_sym)
{
    struct node_cmd *left, *right, *ans;
    left = parse_redir(curr_sym);
    if (left == NULL) {
        return NULL;
    }
    while (*curr_sym == '|') {
        if ((*curr_sym = getchar()) == '|') {
            break;
        }
        ans = malloc(sizeof(struct node_cmd));
        ans->lex = CONVEYOR;
        if (isspace(*curr_sym)) {
            *curr_sym = get_next_sym();
        }
        right = parse_redir(curr_sym);
        ans->left_node = left;
        ans->right_node = right;
        left = ans;
        if (right == NULL) {
            error(MISSING_OPERAND, SET_ERROR, ans);
            return NULL;
        }
    }
    return left;
}

struct node_cmd *
parse_logic(int *curr_sym, int *is_backgrond_symbol)
{
    struct node_cmd *left, *right, *ans;
    left = parse_conveyor(curr_sym);
    if (left == NULL) {
        return NULL;
    }
    while (*curr_sym == '|' || *curr_sym == '&') {
        if (*curr_sym == '&' && (*curr_sym = getchar()) != '&') {
            *is_backgrond_symbol = 1;
            break;
        }
        ans = malloc(sizeof(struct node_cmd));
        if (*curr_sym == '|') {
            ans->lex = DISJUNCTION;
        }
        if (*curr_sym == '&') {
            ans->lex = CONJUNCTION;
        }
        *curr_sym = get_next_sym();
        right = parse_conveyor(curr_sym);
        ans->left_node = left;
        ans->right_node = right;
        left = ans;
        if (right == NULL) {
            error(MISSING_OPERAND, SET_ERROR, ans);
            return NULL;
        }
    }
    return left;
}

struct node_cmd *
parse_separate(int *curr_sym)
{
    struct node_cmd *left, *right, *ans;
    int is_backgrond_symbol = 0;
    left = parse_logic(curr_sym, &is_backgrond_symbol);
    if (left == NULL) {
        return NULL;
    }
    while (is_backgrond_symbol || *curr_sym == ';') {
        ans = malloc(sizeof(struct node_cmd));
        if (is_backgrond_symbol) {
            ans->lex = BACKGROUND_MODE;
        } else {
            ans->lex = SEMICOLON;
        }
        if (!is_backgrond_symbol) {
            *curr_sym = getchar();
        }
        while (isspace(*curr_sym)) {
            *curr_sym = getchar();
        }
        is_backgrond_symbol = 0;
        right = NULL;
        if (*curr_sym != ')') {
            right = parse_separate(curr_sym);
        }
        ans->left_node = left;
        ans->right_node = right;
        left = ans;
        if (right == NULL) {
            break;
        }
    }
    return left;
}

struct node_cmd *
parse_expression(void)
{
    int curr_sym;
    curr_sym = get_next_sym();
    struct node_cmd *ans = parse_separate(&curr_sym);
    switch (curr_sym) {
        case ')':
            error(NO_OPEN_PR, SET_ERROR, NULL);
            break;
        case EOF:
            break;
        default:
            error(MISSING_OPERATION, SET_ERROR, NULL);
            break;
    }
    error(0, CHECK_ERROR, ans);
    set_delete_tree(ans);
    return ans;
}