#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct ilist ilist;
struct ilist {
    int first;
    ilist *rest;
};

ilist *ilist_new(int first, ilist *rest)
{
    ilist *lst = (ilist *)malloc(sizeof(ilist));
    if (lst == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    lst->first = first;
    lst->rest = rest;
    return lst;
}

void ilist_free(ilist *lst)
{
    while (lst != NULL) {
        ilist *next = lst->rest;
        free(lst);
        lst = next;
    }
}
typedef struct graph graph;
struct graph {
    int size;   
    ilist **adj;
};

graph *graph_new(int size)
{
    graph *g = (graph *)malloc(sizeof(graph));
    if (g == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    ilist **adj = (ilist **)malloc(sizeof(ilist *) * size);
    if (adj == NULL) {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        adj[i] = NULL;
    }
    g->size = size;
    g->adj = adj;
    return g;
}

void graph_show(graph *g)
{
    for (int i = 0; i < g->size; i++) {
        ilist *curr = g->adj[i];
        while (curr != NULL) {
            printf("Vertex %d connects to vertex %d\n", i, curr->first);
            curr = curr->rest; //MAKE SURE INCREMENT CORRECTly
        }
    }
}

int in_graph(graph *g, int s, int e)
{
    if (g == NULL) {
        fprintf(stderr,"empty graph\n");
        exit(1);
    }
    ilist *curr = g->adj[s];
    while (curr != NULL) {
        if (curr->first == e) {
            return 1;
        }
        curr = curr->rest;
    }
    return 0;
}

void insert(graph *g, int start, int end)
{
    if (g == NULL) {
        fprintf(stderr,"empty graph\n");
        exit(1);
    }
    ilist *curr = g->adj[start];
    if (curr == NULL) {
        // YOU MUST CHECK NULL CASE
        g->adj[start] = ilist_new(end,NULL);
        return;
    }

    while (curr->rest != NULL) {
        curr = curr->rest;
    }
    curr->rest = ilist_new(end, NULL);
}

void graph_free(graph *g)
{
    for (int i = 0; i < g->size; i++) {
        ilist_free(g->adj[i]);
    }
    free(g->adj);
    free(g);
}

graph *reverse(graph *g)
{
    // reverse for linked list implementation
    if (g == NULL) {
        return g;   
    }
    graph *gr = graph_new(g->size);
    for (int i = 0; i < g->size; i++) {
        ilist *curr = g->adj[i];
        for (; curr != NULL; curr = curr->rest) {
            insert(gr, curr->first,i);
        }
    }
    graph_free(g);
    return gr;
}

int main()
{
    graph *g = graph_new(5);
    insert(g,1,2);
    insert(g,0,4);
    insert(g,0,3);
    insert(g,2,3);
    graph_show(g);

    printf("Edge from %d to %d is in graph: %d\n",0,4,in_graph(g,0,4)); 
    printf("Edge from %d to %d is in graph: %d\n",1,0,in_graph(g,1,0)); 
    printf("Edge from %d to %d is in graph: %d\n",2,3,in_graph(g,2,3)); 

    printf("Reversing graph and printing\n");
    g = reverse(g);
    graph_show(g);
 
    graph_free(g);

    return 0;
}
