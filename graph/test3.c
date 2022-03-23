#include <stdio.h>
#include <limits.h>
#include <string.h>
#include "graph.h"

int
main(void)
{
    Graph *gr = create_graph();
    long num_vert = 5;
    long matrix[5][5] = {
        {0, 4, 0, 7, 0},
        {0, 0, 10, 2, 0},
        {0, 0, 3, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 3, 0, 0}
    };
    for (long i = 0; i < num_vert; ++i) {
        add_vertex(gr);
    }
    for (long i = 0; i < num_vert; ++i) {
        for (long j = 0; j < num_vert; ++j) {
            if (matrix[i][j] != 0) {
                add_edge(gr, i, j, matrix[i][j]);
            }
        }
    }
    delete_vertex(gr, 2);
    int success = 1;

    if (min_dist_between_vertex(gr, 0, 0) != 0) {
        success = 0;
    }
    if (min_dist_between_vertex(gr, 0, 1) != 4) {
        success = 0;
    }
    if (min_dist_between_vertex(gr, 0, 2) != -1) {
        success = 0;
    }
    if (min_dist_between_vertex(gr, 0, 3) != 6) {
        success = 0;
    }
    if (min_dist_between_vertex(gr, 0, 4) != -1) {
        success = 0;
    }
    long path[graph_size(gr)];
    long tmp[graph_size(gr)];
    path_min_dist(gr, 0, 0, path, NULL);
    tmp[0] = 0;
    tmp[1] = -1;
    if (memcmp(path, tmp, 2 * sizeof(long))) {
        success = 0;
    }
    path_min_dist(gr, 0, 1, path, NULL);
    tmp[0] = 0;
    tmp[1] = 1;
    tmp[2] = -1;
    if (memcmp(path, tmp, 3 * sizeof(long))) {
        success = 0;
    }
    path_min_dist(gr, 0, 3, path, NULL);
    tmp[0] = 0;
    tmp[1] = 1;
    tmp[2] = 3;
    tmp[3] = -1;
    if (memcmp(path, tmp, 2 * sizeof(long))) {
        success = 0;
    }

    long dist[graph_size(gr)];
    if (min_dist_between_all_vertex(gr, 2, dist) || min_dist_between_all_vertex(gr, 5, dist)
            || min_dist_between_all_vertex(gr, -1, dist) || min_dist_between_vertex(gr, 2, 1) != -1
            || min_dist_between_vertex(gr, 5, 1) != -1 || min_dist_between_vertex(gr, 1, -1) != -1
            || path_min_dist(gr, -1, 1, path, NULL) || path_min_dist(gr, 2, 1, path, NULL)
            || path_min_dist(gr, 1, 5, path, NULL)) {
        success = 0;
        printf("failure. Impossible action. Passing of incorrect data.\n");
    }

    if (success) {
        printf("\nSuccesfull!\n\n");
    } else {
        printf("\nFailure!\n\n");
    }
    delete_graph(gr);
    return 0;
}