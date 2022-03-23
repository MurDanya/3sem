enum TYPE_ERROR
{
    NO_ERROR,
    NO_OPEN_PR,
    NO_CLOSE_PR,
    MISSING_OPERAND,
    MISSING_OPERATION,
    NO_FILENAME
};

enum CALL_ERROR
{
    SET_ERROR,
    CHECK_ERROR
};

void error(enum TYPE_ERROR, enum CALL_ERROR, tree_cmd *);
void delete_tree(struct node_cmd *);
tree_cmd * set_delete_tree(tree_cmd *);