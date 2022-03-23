struct Edge
{
    long vertex;
    long weight;
};
typedef struct Graph
{
    struct Edge **adj_list;
    long *sizes_vertex;
    long num_vertex;
    long num_alive_vertex;
} Graph;
struct Graph * create_graph(void);
long add_vertex(struct Graph *);
int add_edge(struct Graph *, long, long, long);
int delete_vertex(struct Graph *, long);
int delete_edge(struct Graph *, long, long);
struct Graph * delete_graph(struct Graph *);
int exist_edge(struct Graph *, long, long);
long weight_edge(struct Graph *, long, long);
long graph_size(struct Graph *);
int min_dist_between_all_vertex(struct Graph *, long, long *);
int path_min_dist(struct Graph *, long, long, long *, long *);
long min_dist_between_vertex(struct Graph *, long, long);