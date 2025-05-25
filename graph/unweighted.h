#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct Graph Graph;

Graph *new_graph(size_t vertices_count, bool directed);
bool graph_insert(Graph *g, size_t v1, size_t v2);
bool graph_remove(Graph *g, size_t v1, size_t v2);
void graph_print(Graph *g);
void graph_free(Graph *g);
