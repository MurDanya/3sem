#include <stdio.h>
#include <limits.h>
#include "graph.h"

int
main(void)
{
    Graph *gr = create_graph();
    if (gr == NULL) {
        printf("\nFailure. Memory was not allocated.\n\n");
        return 0;
    }
    long num_vert = 0;
    if (scanf("%ld", &num_vert) && num_vert > 1) {
        for (long i = 0; i < num_vert; ++i) {
            if (add_vertex(gr) == -1) {
                printf("\nFailure. Memory was not allocated.\n\n");
                return 0;
            }
        }
        long weight;
        for (long i = 0; i < num_vert; ++i) {
            for (long j = 0; j < num_vert; ++j) {
                if (scanf("%ld", &weight)) {
                    if (weight != 0) {
                        if (add_edge(gr, i, j, weight) == 0) {
                            printf("\nFailure. Memory was not allocated.\n\n");
                            return 0;
                        }
                    }
                }
            }
        }
        long dist[graph_size(gr)];
        long path[graph_size(gr)];
        long index;
        for (long i = 0; i < num_vert; ++i) {
            min_dist_between_all_vertex(gr, i, dist);
            printf("Minimum path from vertex \"%ld\":\n", i + 1);
            for (long j = 0; j < num_vert; ++j) {
                if (i != j) {
                    printf("to \"%ld\": ",j + 1);
                    if (dist[j] != LONG_MAX) {
                        path_min_dist(gr, i, j, path, dist);
                        index = 0;
                        while (path[index] != -1) {
                            if (index != 0) {
                                printf(" - ");
                            }
                            printf("\"%ld\"", path[index] + 1);
                            ++index;
                        }
                        printf("  (with dist: %ld)\n", dist[j]);
                    } else {
                        printf("NO path\n");
                    }
                }
            }
        }
    }
    return 0;
}