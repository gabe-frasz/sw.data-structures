#include "unweighted.h"
#include <stdio.h>

enum GraphBSFColor { WHITE, GRAY, BLACK };

typedef struct Node {
  size_t vertex;
  struct Node *next;
} Node;

struct Graph {
  size_t vertices_count;
  bool is_directed;
  struct Node **list;
};

bool list_remove(Node **list, size_t vertex);
Node *new_node(size_t vertex);

bool graph_insert(Graph *g, size_t v1, size_t v2) {
  if (g == NULL || g->list == NULL) {
    fprintf(stderr, "graph_insert: graph is NULL\n");
    return false;
  }

  if (v1 >= g->vertices_count || v2 >= g->vertices_count) {
    fprintf(stderr, "graph_insert: invalid vertex index\n");
    return false;
  }

  Node *node = new_node(v2);
  node->next = g->list[v1];
  g->list[v1] = node;

  if (g->is_directed)
    return true;

  node = new_node(v1);
  node->next = g->list[v2];
  g->list[v2] = node;

  return true;
}

void graph_bfs(Graph *g, size_t origin) {
  if (g == NULL || g->list == NULL) {
    fprintf(stderr, "graph_bfs: graph is NULL\n");
    return;
  }

  if (origin >= g->vertices_count) {
    fprintf(stderr, "graph_bfs: invalid vertex index\n");
    return;
  }

  enum GraphBSFColor color[g->vertices_count];
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
    Node *curr_node = g->list[curr];

    while (curr_node != NULL) {
      if (color[curr_node->vertex] == WHITE) {
        color[curr_node->vertex] = GRAY;
        distance[curr_node->vertex] = distance[curr] + 1;
        parent[curr_node->vertex] = curr;
        queue[end++] = curr_node->vertex;
      }
      curr_node = curr_node->next;
    }

    color[curr] = BLACK;
  }

  for (size_t i = 0; i < g->vertices_count; i++) {
    if (parent[i] == -1) {
      printf("v%zu has no parent\n", i);
    } else {
      printf("the parent of v%zu is v%zu\n", i, parent[i]);
    }
  }
}

bool graph_remove(Graph *g, size_t v1, size_t v2) {
  if (g == NULL || g->list == NULL) {
    fprintf(stderr, "graph_remove: graph is NULL\n");
    return false;
  }

  if (v1 >= g->vertices_count || v2 >= g->vertices_count) {
    fprintf(stderr, "graph_remove: invalid vertex index\n");
    return false;
  }

  bool ok = list_remove(&g->list[v1], v2);
  if (g->is_directed || !ok)
    return ok;

  return list_remove(&g->list[v2], v1);
}

void graph_print(Graph *g) {
  if (g == NULL || g->list == NULL) {
    fprintf(stderr, "graph_print: graph is NULL\n");
    return;
  }

  for (size_t i = 0; i < g->vertices_count; i++) {
    printf("v%zu: ", i);
    Node *curr = g->list[i];
    while (curr != NULL) {
      printf("v%zu ", curr->vertex);
      curr = curr->next;
    }
    printf("\n");
  }
}

void graph_free(Graph *g) {
  if (g == NULL)
    return;

  if (g->list == NULL) {
    free(g);
    return;
  }

  for (size_t i = 0; i < g->vertices_count; i++) {
    while (g->list[i] != NULL) {
      Node *tmp = g->list[i];
      g->list[i] = tmp->next;
      free(tmp);
    }
  }

  free(g->list);
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

  graph->list = malloc(sizeof(struct Node *) * vertices_count);
  if (graph->list == NULL) {
    perror("new_graph: failed to allocate memory for graph list");
    exit(EXIT_FAILURE);
  }

  for (size_t i = 0; i < vertices_count; i++)
    graph->list[i] = NULL;

  return graph;
}

// HELPER FUNCTIONS

bool list_remove(Node **list, size_t vertex) {
  if (list == NULL || *list == NULL) {
    fprintf(stderr, "list_remove: list is NULL\n");
    return false;
  }

  Node *curr = *list;
  Node *prev = NULL;

  while (curr != NULL) {
    if (curr->vertex == vertex) {
      if (prev == NULL)
        *list = curr->next;
      else
        prev->next = curr->next;

      free(curr);
      return true;
    }

    prev = curr;
    curr = curr->next;
  }

  fprintf(stderr, "list_remove: vertex not found\n");
  return false;
}

Node *new_node(size_t vertex) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("new_node: failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }
  node->vertex = vertex;
  node->next = NULL;

  return node;
}

int main(void) {
  Graph *g = new_graph(5, false);

  graph_insert(g, 0, 1);
  graph_insert(g, 0, 2);
  graph_insert(g, 1, 2);
  graph_insert(g, 1, 3);
  graph_insert(g, 2, 3);
  graph_insert(g, 2, 4);

  graph_print(g);

  graph_bfs(g, 0);

  graph_free(g);

  g = new_graph(5, true);

  graph_insert(g, 0, 1);
  graph_insert(g, 0, 2);
  graph_insert(g, 1, 2);
  graph_insert(g, 1, 3);
  graph_insert(g, 2, 3);
  graph_insert(g, 2, 4);

  graph_print(g);

  graph_bfs(g, 0);

  graph_free(g);
}
