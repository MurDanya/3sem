#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

void
print_graph(Graph *gr)
{
    for (int i = 0; i < gr->num_vertex; ++i) {
        printf("%d: ", i + 1);
        for (int j = 0; j < gr->sizes_vertex[i]; ++j) {
            printf("%ld(%ld) ", gr->adj_list[i][j].vertex + 1, gr->adj_list[i][j].weight);
        }
        printf("\n");
    }
}

int
main(int argn, char *argv[])
{
    Graph *gr = create_graph();
    long num_vert = 0;
    if (argn > 1 && !strcmp(argv[1], "random")) {
        printf("Number vertex = ");
    }
    if (scanf("%ld", &num_vert) && num_vert > 0) {
        long matrix[num_vert][num_vert];
        for (long i = 0; i < num_vert; ++i) {
            add_vertex(gr);
        }
        if (argn > 1 && !strcmp(argv[1], "random")) {
            for (int i = 0; i < num_vert; ++i) {
                for (int j = 0; j < num_vert; ++j) {
                    matrix[i][j] = rand();
                    if (matrix[i][j] != 0) {
                        add_edge(gr, i, j, matrix[i][j]);
                    }
                }
            }
        } else {
            long weight;
            for (long i = 0; i < num_vert; ++i) {
                for (long j = 0; j < num_vert; ++j) {
                    if (scanf("%ld", &weight)) {
                        matrix[i][j] = weight;
                        if (weight != 0) {
                            add_edge(gr, i, j, weight);
                        }
                    }
                }
            }
        }
        int identical = 1;
        for (int i = 0; i < num_vert; ++i) {
            for (int j = 0; j < num_vert; ++j) {
                if (!((matrix[i][j] == 0 && !exist_edge(gr, i, j))
                        || (matrix[i][j] != 0 && exist_edge(gr, i, j)
                        && matrix[i][j] == weight_edge(gr, i, j)))) {
                    identical = 0;
                }
            }
        }

        if (identical) {
            printf("\nSuccessfull!\n\n");
        } else {
            printf("\nFailure!\n\n");
        }
        delete_graph(gr);
    }
    return 0;
}