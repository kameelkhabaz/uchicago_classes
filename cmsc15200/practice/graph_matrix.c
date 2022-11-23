#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graph graph;
struct graph {
    int size;
    int **matrix;
};

graph *graph_new(int size)
{
    int **matrix = (int **)malloc(sizeof(int *) * size);
    if (matrix == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    for (int i = 0; i < size; i++) {
        matrix[i] = (int *)malloc(sizeof(int) * size);
        if (matrix[i] == NULL) {
            fprintf(stderr,"malloc failed\n");
            exit(1);
        }
        for (int j = 0; j < size; j++) {
            // MUST INITIALIZE MMEMORY MALLOC DOESNT DO THIS FOR U
            matrix[i][j] = 0;
        }
    }
    graph *g = (graph *)malloc(sizeof(graph));
    if (g == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    g->size = size;
    g->matrix = matrix;
    return g;
}

void graph_free(graph *g)
{
    // MUST FREE EACH ROW OF 2D ARRAY AS IT IS ITSELF A POINTER THAT WAS
    // ALLOCATED USING MALLOC
    for (int i = 0; i < g->size; i++) {
        free(g->matrix[i]);
    }
    free(g->matrix);
    free(g);
}

int in_graph(graph *g, int st, int end)
{
    if (g == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    return g->matrix[st][end];
}

void graph_insert(graph *g, int st, int end)
{
    // doesn't check if already in graph
    if (g == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    g->matrix[st][end] = 1;
}

void graph_remove(graph *g, int st, int end)
{
    if (g == NULL) {
        fprintf(stderr,"malloc failed\n");
        exit(1);
    }
    g->matrix[st][end] = 0;
}

void graph_show(graph *g)
{
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            printf("%d ", g->matrix[i][j]);
        }
        printf("\n");
    }   
}

graph *reverse(graph *g)
{
    if (g == NULL) {
        return g;
    }
    graph *gr = graph_new(g->size);
    for (int i = 0; i < g->size; i++) {
        for (int j = 0; j < g->size; j++) {
            gr->matrix[i][j] = g->matrix[j][i];
        }
    }
    graph_free(g);
    return gr;
}

int main()
{
    graph *g = graph_new(5);
    graph_insert(g,2,4);
    graph_insert(g,0,1);
    graph_insert(g,3,0);
    graph_insert(g,4,2);
    graph_insert(g,3,1);
    graph_insert(g,1,0);
    graph_show(g);
    printf("Reversing graph\n");
    g = reverse(g);
    graph_show(g);
    graph_free(g);
    return 0;
}

