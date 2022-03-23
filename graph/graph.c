#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "graph.h"

struct Graph *
create_graph(void)
{
    struct Graph *graph = malloc(sizeof(struct Graph));
    if (graph != NULL) {
        graph->adj_list = NULL;
        graph->sizes_vertex = NULL;
        graph->num_vertex = 0;
        graph->num_alive_vertex = 0;
    }
    return graph;
}

long
add_vertex(struct Graph *graph)
{
    if (graph->num_vertex != graph->num_alive_vertex) {
        long index = 0;
        while (graph->sizes_vertex[index] != -1) {
            ++index;
        }
        graph->sizes_vertex[index] = 0;
        ++graph->num_alive_vertex;
        return index;
    }
    struct Edge **tmp_adj_list = realloc(graph->adj_list,
        (graph->num_vertex + 1) * sizeof(long *));
    long *tmp_sizes_vertex = realloc(graph->sizes_vertex,
        (graph->num_vertex + 1) * sizeof(long));
    if (tmp_adj_list == NULL || tmp_sizes_vertex == NULL) {
        return -1;
    }
    graph->adj_list = tmp_adj_list;
    graph->sizes_vertex = tmp_sizes_vertex;
    graph->adj_list[graph->num_vertex] = NULL;
    graph->sizes_vertex[graph->num_vertex] = 0;
    ++graph->num_vertex;
    ++graph->num_alive_vertex;
    return graph->num_vertex - 1;
}

int
add_edge(struct Graph *graph, long vert1, long vert2, long new_weigth)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return 0;
    }
    struct Edge *mas_edge = graph->adj_list[vert1];
    long index = 0;
    while ((index < graph->sizes_vertex[vert1]) && (mas_edge[index].vertex < vert2)) {
        ++index;
    }
    if ((graph->sizes_vertex[vert1] == index) || (mas_edge[index].vertex != vert2)) {
        mas_edge = realloc(mas_edge, ++graph->sizes_vertex[vert1] * sizeof(struct Edge));
        if (mas_edge == NULL) {
            return 0;
        }
        if (graph->sizes_vertex[vert1] - index - 1 != 0) {
            memmove(mas_edge + index + 1, mas_edge + index,
                (graph->sizes_vertex[vert1] - index - 1) * sizeof(struct Edge));
        }
        mas_edge[index].vertex = vert2;
        mas_edge[index].weight = new_weigth;
        graph->adj_list[vert1] = mas_edge;
    } else {
        mas_edge[index].weight = new_weigth;
    }
    return 1;
}

int
delete_vertex(struct Graph *graph, long vert)
{
    if (vert >= graph->num_vertex || vert < 0 || graph->sizes_vertex[vert] == -1) {
        return 0;
    }
    free(graph->adj_list[vert]);
    graph->adj_list[vert] = NULL;
    graph->sizes_vertex[vert] = -1;
    for (long i = 0; i < graph->num_vertex; ++i) {
        if (graph->adj_list[i] != NULL && graph->sizes_vertex[i] > 0) {
            for (long j = 0; j < graph->sizes_vertex[i]; ++j) {
                if (graph->adj_list[i][j].vertex == vert) {
                    if (graph->sizes_vertex[i] - j - 1 != 0) {
                        memmove(graph->adj_list[i] + j, graph->adj_list[i] + j + 1,
                            (graph->sizes_vertex[i] - j - 1) * sizeof(struct Edge));
                    }
                    graph->adj_list[i] = realloc(graph->adj_list[i],
                        --graph->sizes_vertex[i] * sizeof(struct Edge));
                    break;
                }
            }
        }
    }
    --graph->num_alive_vertex;
    return 1;
}

int
delete_edge(struct Graph *graph, long vert1, long vert2)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return 0;
    }
    struct Edge *mas_edge = graph->adj_list[vert1];
    long index = 0;
    while ((index < graph->sizes_vertex[vert1] - 1) && (mas_edge[index].vertex < vert2)) {
        ++index;
    }
    if ((graph->sizes_vertex[vert1] != 0) && (mas_edge[index].vertex == vert2)) {
        if (graph->sizes_vertex[vert1] - index - 1 != 0) {
            memmove(mas_edge + index, mas_edge + index + 1,
                (graph->sizes_vertex[vert1] - index - 1) * sizeof(struct Edge));
        }
        mas_edge = realloc(mas_edge, --graph->sizes_vertex[vert1] * sizeof(struct Edge));
        graph->adj_list[vert1] = mas_edge;
        return 1;
    }
    return 0;
}

struct Graph *
delete_graph(struct Graph *graph)
{
    if (graph != NULL) {
        if (graph->adj_list != NULL) {
            for (int i = 0; i < graph->num_vertex; ++i) {
                free(graph->adj_list[i]);
            }
        }
        free(graph->adj_list);
        free(graph->sizes_vertex);
        free(graph);
    }
    return NULL;
}

int
exist_edge(struct Graph* graph, long vert1, long vert2)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return 0;
    }
    struct Edge *mas_edge = graph->adj_list[vert1];
    for (int i = 0; i < graph->sizes_vertex[vert1]; ++i) {
        if (mas_edge[i].vertex == vert2) {
            return 1;
        } else if (mas_edge[i].vertex > vert2) {
            return 0;
        }
    }
    return 0;
}

long
weight_edge(struct Graph* graph, long vert1, long vert2)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return -1;
    }
    struct Edge *mas_edge = graph->adj_list[vert1];
    for (int i = 0; i < graph->sizes_vertex[vert1]; ++i) {
        if (mas_edge[i].vertex == vert2) {
            return mas_edge[i].weight;
        } else if (mas_edge[i].vertex > vert2) {
            return -1;
        }
    }
    return -1;
}

long
graph_size(struct Graph *graph)
{
    return graph->num_vertex + 1;
}

int
min_dist_between_all_vertex (struct Graph *graph, long vert, long *dist)
{
    if (vert >= graph->num_vertex || vert < 0 || graph->sizes_vertex[vert] == -1) {
        return 0;
    }
    char visit_vert[graph->num_vertex];
    long min_dist, min_dist_index;
    for (long i = 0; i < graph->num_vertex; ++i) {
        dist[i] = LONG_MAX;
        visit_vert[i] = 1;
    }
    dist[vert] = 0;
    do {
        min_dist = LONG_MAX;
        min_dist_index = LONG_MAX;
        for (long i = 0; i < graph->num_vertex; ++i) {
            if ((visit_vert[i] == 1) && (dist[i] < min_dist)) {
                min_dist = dist[i];
                min_dist_index = i;
            }
        }
        if (min_dist_index != LONG_MAX) {
            for (int i = 0; i < graph->sizes_vertex[min_dist_index]; ++i) {
                long tmp = min_dist + graph->adj_list[min_dist_index][i].weight;
                if (tmp < dist[graph->adj_list[min_dist_index][i].vertex]) {
                    dist[graph->adj_list[min_dist_index][i].vertex] = tmp;
                }
            }
            visit_vert[min_dist_index] = 0;
        }
    } while (min_dist_index != LONG_MAX);
    return 1;
}

int
path_min_dist(struct Graph *graph, long vert1, long vert2, long *path, long *user_dist)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return 0;
    }
    long dist[graph->num_vertex + 1];
    if (user_dist == NULL) {
        min_dist_between_all_vertex(graph, vert1, dist);
    } else {
        memcpy(dist, user_dist, graph->num_vertex * sizeof(long));
    }
    long path_min_dist[graph->num_vertex + 1];
    long end = vert2;
    path_min_dist[0] = end;
    long index = 0;
    long weight_end = dist[end];
    while (end != vert1) {
        long i;
        for(i = 0; i < graph->num_vertex; ++i) {
            if (exist_edge(graph, i, end)) {
                long tmp = weight_end - weight_edge(graph, i, end);
                if (tmp == dist[i]) {
                    weight_end = tmp;
                    end = i;
                    path_min_dist[++index] = i;
                    break;
                }
            }
        }
        if (i == graph->num_vertex || weight_end < 0) {
            return 0;
        }
    } 
    for (int i = index; i >= 0; --i) {
        *path = path_min_dist[i];
        ++path;
    }
    *path = -1;
    return 1;
}

long
min_dist_between_vertex(struct Graph *graph, long vert1, long vert2)
{
    if (vert1 >= graph->num_vertex || vert1 < 0 || graph->sizes_vertex[vert1] == -1
            || vert2 >= graph->num_vertex || vert2 < 0 || graph->sizes_vertex[vert2] == -1) {
        return -1;
    }
    long dist[graph->num_vertex + 1];
    min_dist_between_all_vertex(graph, vert1, dist);
    return (dist[vert2] != LONG_MAX)?(dist[vert2]):(-1);
}