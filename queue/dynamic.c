#include "dynamic.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct Queue {
  Node *start;
  Node *end;
  size_t size;
} Queue;

bool enqueue(Queue *q, int value) {
  if (q == NULL) {
    fprintf(stderr, "enqueue: queue is NULL\n");
    return false;
  }

  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("enqueue: failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }
  node->value = value;
  node->next = NULL;

  if (q->start == NULL)
    q->start = node;
  else
    q->end->next = node;

  q->end = node;
  q->size++;

  return true;
}

bool dequeue(Queue *q, int *value) {
  if (q == NULL || q->size == 0) {
    fprintf(stderr, "dequeue: queue is NULL or empty\n");
    return false;
  }

  Node *node = q->start;
  *value = node->value;

  q->start = node->next;
  if (q->start == NULL)
    q->end = NULL;

  free(node);
  q->size--;

  return true;
}

bool queue_peek(Queue *q, int *value) {
  if (q == NULL || q->size == 0) {
    fprintf(stderr, "queue_peek: queue is NULL or empty\n");
    return false;
  }

  *value = q->start->value;
  return true;
}

void queue_print(Queue *q) {
  if (q == NULL || q->size == 0)
    return;

  Node *node = q->start;

  printf("Start: ");
  while (node != NULL) {
    printf("%d ", node->value);
    node = node->next;
  }
  printf("\n");
}

void queue_free(Queue *q) {
  if (q == NULL)
    return;

  while (q->start != NULL) {
    Node *tmp = q->start;
    q->start = q->start->next;
    free(tmp);
  }
  free(q);
}

Queue *new_queue() {
  Queue *q = malloc(sizeof(Queue));
  if (q == NULL) {
    perror("new_queue: failed to allocate memory for queue");
    exit(EXIT_FAILURE);
  }
  q->start = NULL;
  q->end = NULL;
  q->size = 0;

  return q;
}
