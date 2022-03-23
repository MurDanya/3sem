#include <stdio.h>
#include "graph.h"

int
main(void)
{
    printf("\n");
    int test_success = 1;
    Graph *gr = create_graph();
    if (gr != NULL) {
        printf("success");
    } else {
        test_success = 0;
        printf("failure. Memory was not allocated.");
    }
    printf (" (create graph)\n");



    long ver1 = add_vertex(gr);
    long ver2 = add_vertex(gr);
    long weight = 12;
    if (ver1 == -1 || ver2 == -1) {
        test_success = 0;
        printf("failure. Memory was not allocated.");
    } else {
        if (!add_edge(gr, ver1, ver2, weight)) {
            test_success = 0;
            printf("failure. Memory was not allocated.");
        } else {
            if (!exist_edge(gr, ver1, ver2)) {
                test_success = 0;
                printf("failure. The edge was not added.");
            } else {
                if (weight_edge(gr, ver1, ver2) != weight) {
                    test_success = 0;
                    printf("failure. Incorrect edge weight.");
                } else {
                    printf("success");
                }
            }
        }
    }
    printf (" (add vertexs and add edge)\n");



    if (!exist_edge(gr, ver2, ver1)) {
        printf("success");
    } else {
        test_success = 0;
        printf("failure. Added an extra edge.");
    }
    printf(" (checking the orientation of the graph)\n");



    long ver3 = add_vertex(gr);
    if (ver3 == -1) {
        test_success = 0;
        printf("failure. Memory was not allocated.");
    } else {
        weight = 101;
        if (!add_edge(gr, ver2, ver3, weight)) {
            test_success = 0;
            printf("failure. Memory was not allocated.");
        } else {
            if (!exist_edge(gr, ver2, ver3)) {
                test_success = 0;
                printf("failure. The edge was not added.");
            } else {
                if (weight_edge(gr, ver2, ver3) != weight) {
                    test_success = 0;
                    printf("failure. Incorrect edge weight.");
                } else {
                    printf("success");
                }
            }
        }
    }
    printf(" (add new vertex and add new edge)\n");



    weight = 23;
    if (!add_edge(gr, ver1, ver2, weight)) {
        test_success = 0;
        printf("failure. The edge has not changed.");
    } else {
        if (!exist_edge(gr, ver1, ver2)) {
            test_success = 0;
            printf("failure. The edge has been removed.");
        } else {
            if (weight_edge(gr, ver1, ver2) != weight) {
                test_success = 0;
                printf("failure. The weight of the edge has not changed.");
            } else {
                printf("success");
            }
        }
    }
    printf(" (changing an existing edge)\n");



    if (delete_edge(gr, ver1, ver2) == 0) {
        test_success = 0;
        printf("failure. The edge has not removed.");
    } else {
        if (exist_edge(gr, ver1, ver2)) {
            test_success = 0;
            printf("failure. The edge has not removed.");
        } else {
            printf("success");
        }
    }
    printf(" (deleting an edge)\n");



    weight = 10;
    if (!add_edge(gr, ver3, ver2, weight)) {
        test_success = 0;
        printf("failure. Memory was not allocated.");
    } else {
        if (!exist_edge(gr, ver3, ver2)) {
            test_success = 0;
            printf("failure. The edge was not added.");
        } else {
            if (weight_edge(gr, ver3, ver2) != weight) {
                test_success = 0;
                printf("failure. Incorrect edge weight.");
            } else {
                printf("success");
            }
        }
    }
    printf(" (add new edge)\n");




    if (delete_vertex(gr, ver3) == 0) {
        test_success = 0;
        printf("failure. The vertex has not removed.");
    } else {
        if (add_edge(gr, ver1, ver3, 1)) {
            test_success = 0;
            printf("failure. The vertex has not removed.");
        } else {
            if (exist_edge(gr, ver3, ver2)) {
                test_success = 0;
                printf("failure. The edges of the deleted vertex were not removed.");
            } else {
                if (exist_edge(gr, ver2, ver3)) {
                    test_success = 0;
                    printf("failure. The edges of the deleted vertex were not removed.");
                } else {
                    printf("success");
                }
            }
        }
    }
    printf(" (deleting an vertex)\n");



    if (add_edge(gr, ver1, -1, 1) || add_edge(gr, ver1, ver3, 1) || add_edge(gr, ver1, ver3 + 1, 1)
            || add_edge(gr, -1, ver1, 1) || delete_vertex(gr, ver3) || delete_vertex(gr, ver3 + 1)
            || delete_vertex(gr, -1) || delete_edge(gr, ver1, ver2) || delete_edge(gr, ver2, ver1)
            || delete_edge(gr, ver1, ver3) || delete_edge(gr, ver1, ver3 + 1)
            || delete_edge(gr, -1, ver1)) {
        test_success = 0;
        printf("failure. Impossible action. Passing of incorrect data.");
    } else {
        printf("success");
    }
    printf(" (checking the passing of incorrect data)\n");



    long ver4 = add_vertex(gr);
    if (ver3 != ver4) {
        test_success = 0;
        printf("failure. Incorrect addition of a new vertex.");
    } else {
        printf("success");
    }
    printf(" (adding a new vertex after deleting an old one)\n");



    add_edge(gr, ver1, ver2, 1);
    add_edge(gr, ver2, ver3, 2);
    add_edge(gr, ver3, ver1, 3);
    if (delete_graph(gr) == NULL) {
        printf("success");
    } else {
        test_success = 0;
        printf("failure");
    }
    printf(" (deleting a graph)\n");



    if (test_success) {
        printf("\nTest passed successfully!\n\n");
    } else {
        printf("\nTest failed\n\n");
    }
    return 0;
}