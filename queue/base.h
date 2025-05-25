#pragma once

#include <stdbool.h>

typedef struct Queue Queue;

bool enqueue(Queue *q, int value);
bool dequeue(Queue *q, int *value);
bool queue_peek(Queue *q, int *value);
void queue_print(Queue *q);
void queue_free(Queue *q);
