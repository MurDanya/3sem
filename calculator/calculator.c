#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <errno.h>

enum Type_elem {NUM_INT, VAR, OPERATION};

union Elem
{
    double num_double;
    int num_int;
    char var[7];
    char opertion;
};

struct Elem_POLIS
{
    enum Type_elem type_elem;
    union Elem elem;
};

struct POLIS
{
    struct Elem_POLIS *elements;
    int size_polis;
};

struct Elem_Var
{
    char name[7];
    int val;
};

struct Variables
{
    struct Elem_Var *vars;
    int num_variables;
};

enum Sym_value
{
    PLUS = '+',
    MINUS = '-',
    MUL = '*',
    DIV = '/',
    END = '\n',
    LP = '(',
    RP = ')',
    UNDERSCORE = '_'
};

int curr_sym = LP;
struct POLIS *polis = NULL;
int size_var_polis = 0;
struct Variables *variables = NULL;
int size_var_variables = 0;
int *stack = NULL;
int size_var_stack = 0;

int get_size_int(void);
void polis_extension(void);
void variables_extension(void);
void error(const char *, int);
void skip_spaces(void);
void read_number(void);
void read_variable(void);
void put_opertion_in_polis(char);
void get_sym(void);
void prim(void);
void term(void);
void expr(void);
void parse(void);
void read_all_variables(void);
void stack_extension(int);
int get_index_var(char *);
int calculation_polis(void);
void free_memory(void);

int
get_size_int(void)
{
    static int size_int = 0;
    if (!size_int) {
        int tmp = INT_MAX;
        while (tmp > 0) {
            tmp /= 10;
            ++size_int;
        }
    }
    return size_int;
}

void
polis_extension(void)
{
    ++polis->size_polis;
    if (polis->size_polis > size_var_polis) {
        size_var_polis = (size_var_polis)?(2 * size_var_polis):(1);
        polis->elements = realloc(polis->elements, size_var_polis * sizeof(*polis->elements));
    }
}

void
variables_extension(void)
{
    ++variables->num_variables;
    if (variables->num_variables > size_var_variables) {
        size_var_variables = (size_var_variables)?(2 * size_var_variables):(1);
        variables->vars = realloc(variables->vars, size_var_variables * sizeof(*variables->vars));
    }
}

void
error(const char *str, int num_exit_code)
{
    fprintf(stderr, "Error: %s\n", str);
    exit(num_exit_code);
}

void
skip_spaces(void)
{
    char tmp = curr_sym;
    while ((isspace(curr_sym) && curr_sym != END) || curr_sym == EOF) {
        if (curr_sym == EOF) {
            error("End of file, there is no sign of the end of the expression", 1);
        }
        curr_sym = getchar();
    }
    switch (curr_sym) {
        case END:
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case RP:
            break;
        default:
            if (isspace(tmp) || curr_sym == LP) {
                error("Missing operation", 1);
            } else {
                error("Invalid operand", 1);
            }
    }
    return;
}

void
read_number(void)
{
    int size_number = get_size_int();
    char number_in_string[size_number + 1];
    int index = 0;
    do {
        number_in_string[index] = curr_sym;
        curr_sym = getchar();
        ++index;
        if (index >= size_number) {
            break;
        }
    } while (isdigit(curr_sym));
    number_in_string[index] = '\0';
    if (isdigit(curr_sym)) {
        error("The constant goes beyond the range of the INT type", 1);
    }
    polis_extension();
    long tmp = strtol(number_in_string, NULL, 10);
    if (LONG_MAX == INT_MAX) {
        if (errno == ERANGE && (tmp == LONG_MAX || tmp == LONG_MIN)) {
            error("The constant goes beyond the range of the INT type", 1);
        }
    } else {
        if (tmp > INT_MAX || tmp < INT_MIN) {
            error("The constant goes beyond the range of the INT type", 1);
        }
    }
    polis->elements[polis->size_polis - 1].elem.num_int = tmp;
    polis->elements[polis->size_polis - 1].type_elem = NUM_INT;
    skip_spaces(); 
    return;
}

void
read_variable(void)
{
    char name[7];
    int index;
    for (index = 0; index < 6 && (isalnum(curr_sym) || curr_sym == UNDERSCORE); ++index) {
        name[index] = curr_sym;
        curr_sym = getchar();
    }
    name[index] = '\0';
    skip_spaces();
    polis_extension();
    polis->elements[polis->size_polis - 1].type_elem = VAR;
    strcpy(polis->elements[polis->size_polis - 1].elem.var, name);
    int tmp_result;
    for (index = 0; index < variables->num_variables; ++index) {
        if ((tmp_result = strcmp(variables->vars[index].name, name)) > 0) {
            variables_extension();
            memmove(variables->vars + index + 1, variables->vars + index,
                (variables->num_variables - index - 1) * sizeof(struct Elem_Var));
            strcpy(variables->vars[index].name, name);
            return;
        } else if (tmp_result == 0) {
            return;
        }
    }
    variables_extension();
    strcpy(variables->vars[variables->num_variables - 1].name, name);
    return;
}

void
put_opertion_in_polis(char oper)
{
    polis_extension();
    polis->elements[polis->size_polis - 1].type_elem = OPERATION;
    polis->elements[polis->size_polis - 1].elem.opertion = oper;
    return;
}

void
get_sym(void)
{
    do {
        if ((curr_sym = getchar()) == EOF) {
            error("End of file, There is no sign of the end of the expression", 1);
        }
    } while (curr_sym != END && isspace(curr_sym));
    return;
}


void
prim(void)
{
    get_sym();
    switch (curr_sym) {
        case LP:
            expr();
            if (curr_sym != RP) {
                error("No closing parenthesis ')'", 1);
            }
            get_sym();
            return;
        case END:
        case PLUS:
        case MINUS:
        case MUL:
        case DIV:
        case RP:
            error("Missing operand", 1);
        default:
            if (isdigit(curr_sym)) {
                read_number();
                return;
            }
            if (isalpha(curr_sym) || curr_sym == UNDERSCORE) {
                read_variable();
                return;
            }
            error("Invalid operand", 1);
    }
}

void
term(void)
{
    prim();
    while (1) {
        switch (curr_sym) {
            case MUL:
                prim();
                put_opertion_in_polis(MUL);
                break;
            case DIV:
                prim();
                put_opertion_in_polis(DIV);
                break;
            default:
                return;
        }
    }
}

void
expr(void)
{
    term();
    while (1) {
        switch (curr_sym) {
            case PLUS:
                term();
                put_opertion_in_polis(PLUS);
                break;
            case MINUS:
                term();
                put_opertion_in_polis(MINUS);
                break;
            default:
                return;
        }
    }
}

void
parse(void)
{
    expr();
    switch (curr_sym) {
        case RP:
            error("No opening parenthesis '('", 1);
        case END:
            break;
        default:
            error("Missing operation", 1);
    }
}

void
read_all_variables(void)
{
    for (int i = 0; i < variables->num_variables; ++i) {
        do {
            curr_sym = getchar();
            if (curr_sym == END) {
                error("The value of the variable is missing", 2);
            }
        } while (isspace(curr_sym));
        if (!isdigit(curr_sym) && curr_sym != MINUS) {
            error("Invalid variable value", 2);
        }
        int size_number = get_size_int();
        char number_in_string[size_number + 2];
        int index = 0;
        do {
            number_in_string[index] = curr_sym;
            curr_sym = getchar();
            ++index;
            if (index >= size_number + 1) {
                break;
            }
        } while (isdigit(curr_sym));
        number_in_string[index] = '\0';
        if (isdigit(curr_sym)) {
            error("The value goes beyond the range of the INT type", 2);
        }
        while (isspace(curr_sym)) {
            if (curr_sym == END) {
                break;
            }
            curr_sym = getchar();
        }
        if (curr_sym == EOF) {
            error("End of file, There is no sign of the end of the value", 2);
        }
        if (curr_sym != END || (index == 1 && number_in_string[0] == MINUS)) {
            error("Invalid variable value", 2);
        }
        long tmp = strtol(number_in_string, NULL, 10);
        if (LONG_MAX == INT_MAX) {
            if (errno == ERANGE && (tmp == LONG_MAX || tmp == LONG_MIN)) {
                error("The value goes beyond the range of the INT type", 2);
            }
        } else {
            if (tmp > INT_MAX || tmp < INT_MIN) {
                error("The value goes beyond the range of the INT type", 2);
            }
        }
        variables->vars[i].val = tmp;
    }
}

void
stack_extension(int size_stack)
{
    if (size_stack > size_var_stack) {
        size_var_stack = (size_var_stack)?(2 * size_var_stack):(1);
        stack = realloc(stack, size_var_stack * sizeof(*stack));
    }
}

int
get_index_var(char *name)
{
    for (int i = 0; i < variables->num_variables; ++i) {
        if (!strcmp(variables->vars[i].name, name)) {
            return i;
        }
    }
    return -1;
}

int
calculation_polis(void)
{
    int top_stack = 0;
    int index, left, right;
    for (int i = 0; i < polis->size_polis; ++i) {
        switch (polis->elements[i].type_elem) {
            case NUM_INT:
                ++top_stack;
                stack_extension(top_stack);
                stack[top_stack - 1] = polis->elements[i].elem.num_int;
                break;
            case VAR:
                index = get_index_var(polis->elements[i].elem.var);
                ++top_stack;
                stack_extension(top_stack);
                stack[top_stack - 1] = variables->vars[index].val;
                break;
            case OPERATION:
                left = stack[top_stack - 2];
                right = stack[top_stack - 1];
                switch (polis->elements[i].elem.opertion) {
                    case PLUS:
                        if ((right > 0 && left > (INT_MAX - right))
                                || (right < 0 && left < (INT_MIN - right))) {
                            error("Overflow during addition", 3);
                        }
                        left += right;
                        break;
                    case MINUS:
                        if ((right > 0 && left < (INT_MIN + right))
                                || (right < 0 && left > (INT_MAX + right))) {
                            error("Overflow during subtraction", 3);
                        }
                        left -= right;
                        break;
                    case MUL:
                        if ((right > 0 && left > 0 && right > (INT_MAX / left))
                                || (right > 0 && left < 0 && left < (INT_MIN / right))
                                || (right < 0 && left > 0 && right < (INT_MIN / left))
                                || (right < 0 && left < 0 && left < (INT_MAX / right))) {
                            error("Overflow during multiplication", 3);
                        }
                        left *= right;
                        break;
                    case DIV:
                        if (right == 0) {
                            error("Division by 0", 3);
                        }
                        if (left == INT_MIN && right == -1) {
                            error("Overflow during division", 3);
                        }
                        left /= right;
                        break;
                    default:
                        break;
                }
                stack[top_stack - 2] = left;
                --top_stack;
            default:
                break;
        }
    }
    return stack[0];
}



void
free_memory(void)
{
    free(variables->vars);
    free(variables);
    free(polis->elements);
    free(polis);
    free(stack);
}

int
main(void)
{
    polis = malloc(sizeof(*polis));
    polis->size_polis = 0;
    polis->elements = NULL;
    variables = malloc(sizeof(*variables));
    variables->num_variables = 0;
    variables->vars = NULL;
    atexit(free_memory);
    parse();
    read_all_variables();
    int answer = calculation_polis();
    printf("%d\n", answer);
    exit(0);
}