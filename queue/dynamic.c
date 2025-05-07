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

int enqueue(Queue *q, int value) {
  if (q == NULL)
    return 0;

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
  return 1;
}

int dequeue(Queue *q, int *value) {
  if (q == NULL || q->size == 0)
    return 0;

  Node *node = q->start;
  *value = node->value;

  q->start = node->next;
  if (q->start == NULL)
    q->end = NULL;

  free(node);
  q->size--;
  return 1;
}

int queue_peek(Queue *q, int *value) {
  if (q == NULL || q->size == 0)
    return 0;

  *value = q->start->value;
  return 1;
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

  Node *node = q->start;
  while (node != NULL) {
    Node *tmp = node;
    node = node->next;
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
