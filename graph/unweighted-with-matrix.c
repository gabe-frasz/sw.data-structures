#include "unweighted.h"
#include <stdio.h>

enum GraphBFSColor { WHITE, GRAY, BLACK };

struct Graph {
  size_t vertices_count;
  bool is_directed;
  int **matrix;
};

bool graph_insert(Graph *g, size_t v1, size_t v2) {
  if (g == NULL || g->matrix == NULL) {
    fprintf(stderr, "graph_insert: graph is NULL\n");
    return false;
  }

  if (v1 >= g->vertices_count || v2 >= g->vertices_count) {
    fprintf(stderr, "graph_insert: invalid vertex index\n");
    return false;
  }

  g->matrix[v1][v2] = 1;

  if (g->is_directed)
    return true;

  g->matrix[v2][v1] = 1;

  return true;
}

void graph_bfs(Graph *g, size_t origin) {
  if (g == NULL || g->matrix == NULL) {
    fprintf(stderr, "graph_bfs: graph is NULL\n");
    return;
  }

  if (origin >= g->vertices_count) {
    fprintf(stderr, "graph_bfs: invalid vertex index\n");
    return;
  }

  enum GraphBFSColor color[g->vertices_count];
  size_t distance[g->vertices_count];
  size_t parent[g->vertices_count];

  for (size_t i = 0; i < g->vertices_count; i++) {
    color[i] = WHITE;
    distance[i] = -1;
    parent[i] = -1;
  }

  color[origin] = GRAY;
  distance[origin] = 0;

  size_t queue[g->vertices_count];
  size_t start = 0, end = 0;

  queue[end++] = origin;

  while (start < end) {
    size_t curr = queue[start++];

    for (size_t i = 0; i < g->vertices_count; i++) {
      if (g->matrix[curr][i] && color[i] == WHITE) {
        color[i] = GRAY;
        distance[i] = distance[curr] + 1;
        parent[i] = curr;
        queue[end++] = i;
      }
    }

    color[curr] = BLACK;
  }

  for (size_t i = 0; i < g->vertices_count; i++) {
    if (parent[i] == -1)
      printf("v%zu has no parent\n", i);
    else
      printf("the parent of v%zu is v%zu\n", i, parent[i]);
  }
}

bool graph_remove(Graph *g, size_t v1, size_t v2) {
  if (g == NULL || g->matrix == NULL) {
    fprintf(stderr, "graph_remove: graph is NULL\n");
    return false;
  }

  if (v1 >= g->vertices_count || v2 >= g->vertices_count) {
    fprintf(stderr, "graph_remove: invalid vertex index\n");
    return false;
  }

  g->matrix[v1][v2] = 0;

  if (g->is_directed)
    return true;

  g->matrix[v2][v1] = 0;

  return true;
}

void graph_print(Graph *g) {
  if (g == NULL || g->matrix == NULL) {
    fprintf(stderr, "graph_print: graph is NULL\n");
    return;
  }

  for (size_t i = 0; i < g->vertices_count; i++) {
    printf("v%zu: ", i);
    for (size_t j = 0; j < g->vertices_count; j++)
      if (g->matrix[i][j])
        printf("v%zu ", j);
    printf("\n");
  }
}

void graph_free(Graph *g) {
  if (g == NULL)
    return;

  if (g->matrix != NULL) {
    for (size_t i = 0; i < g->vertices_count; i++)
      free(g->matrix[i]);
    free(g->matrix);
  }

  free(g);
}

Graph *new_graph(size_t vertices_count, bool directed) {
  Graph *graph = malloc(sizeof(Graph));
  if (graph == NULL) {
    perror("new_graph: failed to allocate memory for graph");
    exit(EXIT_FAILURE);
  }

  graph->vertices_count = vertices_count;
  graph->is_directed = directed;

  graph->matrix = malloc(sizeof(int *) * vertices_count);
  if (graph->matrix == NULL) {
    perror("new_graph: failed to allocate memory for graph matrix");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < vertices_count; i++) {
    graph->matrix[i] = malloc(sizeof(int) * vertices_count);
    if (graph->matrix[i] == NULL) {
      perror("new_graph: failed to allocate memory for graph matrix");
      exit(EXIT_FAILURE);
    }

    for (size_t j = 0; j < vertices_count; j++)
      graph->matrix[i][j] = 0;
  }

  return graph;
}

int main(void) {
  Graph *g = new_graph(5, false);
  graph_insert(g, 0, 1);
  graph_insert(g, 0, 2);
  graph_insert(g, 1, 2);
  graph_insert(g, 2, 3);
  graph_insert(g, 3, 4);
  graph_insert(g, 4, 0);

  graph_print(g);
  graph_bfs(g, 0);

  graph_remove(g, 0, 1);
  graph_remove(g, 0, 2);
  graph_remove(g, 1, 2);
  graph_remove(g, 2, 3);
  graph_remove(g, 3, 4);
  graph_remove(g, 4, 0);

  graph_print(g);

  graph_free(g);

  g = new_graph(5, true);
  graph_insert(g, 0, 1);
  graph_insert(g, 0, 2);
  graph_insert(g, 1, 2);
  graph_insert(g, 2, 3);
  graph_insert(g, 3, 4);

  graph_print(g);
  graph_bfs(g, 0);

  graph_remove(g, 0, 1);
  graph_remove(g, 0, 2);
  graph_remove(g, 1, 2);
  graph_remove(g, 2, 3);
  graph_remove(g, 3, 4);
  graph_remove(g, 4, 0);

  graph_print(g);

  graph_free(g);
}
