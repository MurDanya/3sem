#include <stdio.h>

#include "recursive_descant.h"
#include "error_delete.h"
#include "shell.h"

int
main(void)
{
    tree_cmd *tree = parse_expression();
    int exit_code = run_tree(tree);
    return exit_code;
}