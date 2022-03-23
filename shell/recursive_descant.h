enum LEXEM_TYPE
{
    DISJUNCTION,
    CONJUNCTION,
    CONVEYOR,
    SEMICOLON,
    BACKGROUND_MODE,
    REDIRECTION,
    PROGRAM
};

typedef struct node_cmd
{
    enum LEXEM_TYPE lex;
    char **prog;
    char **redirection;
    struct node_cmd *left_node;
    struct node_cmd *right_node;
} tree_cmd;

struct node_cmd * parse_expression(void);