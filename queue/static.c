#include "static.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
  int *data;
  int start;
  int size;
  int max_size;
} Queue;

bool enqueue(Queue *q, int value) {
  if (q == NULL || q->size == q->max_size) {
    fprintf(stderr, "enqueue: queue is NULL or full\n");
    return false;
  }

  int end = q->start == -1 ? ++q->start : (q->start + q->size) % q->max_size;
  q->data[end] = value;
  q->size++;

  return true;
}

bool dequeue(Queue *q, int *value) {
  if (q == NULL || q->size == 0) {
    fprintf(stderr, "dequeue: queue is NULL or empty\n");
    return false;
  }

  *value = q->data[q->start];
  q->start = (q->start + 1) % q->max_size;
  q->size--;

  return true;
}

bool queue_peek(Queue *q, int *value) {
  if (q == NULL || q->size == 0) {
    fprintf(stderr, "queue_peek: queue is NULL or empty\n");
    return false;
  }

  *value = q->data[q->start];
  return true;
}

void queue_print(Queue *q) {
  if (q == NULL || q->size == 0)
    return;

  printf("Start: ");
  for (int i = q->start; i < q->start + q->size; i++) {
    printf("%d ", q->data[i % q->max_size]);
  }
  printf("\n");
}

void queue_free(Queue *q) {
  if (q == NULL)
    return;

  if (q->data != NULL)
    free(q->data);

  free(q);
}

Queue *new_queue(int max_size) {
  Queue *q = malloc(sizeof(Queue));
  if (q == NULL) {
    perror("new_queue: failed to allocate memory for queue");
    exit(EXIT_FAILURE);
  }

  q->data = malloc(sizeof(int) * max_size);
  if (q->data == NULL) {
    perror("new_queue: failed to allocate memory for queue data");
    exit(EXIT_FAILURE);
  }

  q->start = -1;
  q->size = 0;
  q->max_size = max_size;

  return q;
}
