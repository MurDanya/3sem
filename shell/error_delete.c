#include <stdio.h>
#include <stdlib.h>

#include "recursive_descant.h"
#include "error_delete.h"

void
error(enum TYPE_ERROR type_err, enum CALL_ERROR call_err, tree_cmd *tree)
{
    static enum TYPE_ERROR cur_type_err = NO_ERROR;
    if (call_err == SET_ERROR) {
        delete_tree(tree);
        if (cur_type_err == NO_ERROR) {
            cur_type_err = type_err;
        }
    }
    if (call_err == CHECK_ERROR && cur_type_err != NO_ERROR) {
        if (cur_type_err == NO_OPEN_PR) {
            fprintf(stderr, "shell: No opening parenthesis '('\n");
        }
        if (cur_type_err == NO_CLOSE_PR) {
            fprintf(stderr, "shell: No closing parenthesis ')'\n");
        }
        if (cur_type_err == MISSING_OPERAND) {
            fprintf(stderr, "shell: Missing operand\n");
        }
        if (cur_type_err == MISSING_OPERATION) {
            fprintf(stderr, "shell: Missing operation\n");
        }
        if (cur_type_err == NO_FILENAME) {
            fprintf(stderr, "shell: No file name in the redirection\n");
        }
        delete_tree(tree);
        exit(1);
    }
    return;
}

void
delete_tree(struct node_cmd *node)
{
    if (node != NULL) {
        delete_tree(node->left_node);
        delete_tree(node->right_node);
        if (node->lex == REDIRECTION) {
            free(node->redirection[0]);
            free(node->redirection[1]);
            free(node->redirection[2]);
            free(node->redirection);
        }
        if (node->lex == PROGRAM) {
            int index = 0;
            while (node->prog[index] != NULL) {
                free(node->prog[index]);
                ++index;
            }
            free(node->prog);
        }
        free(node);
    }
    return;
}

void
delete_tree_when_exit(void)
{
    tree_cmd *tree = set_delete_tree(NULL);
    delete_tree(tree);
}

tree_cmd *
set_delete_tree(tree_cmd *tree)
{
    static tree_cmd *tree_which_need_delete = NULL;
    if (tree != NULL) {
        tree_which_need_delete = tree;
        atexit(delete_tree_when_exit);
    }
    return tree_which_need_delete;
}