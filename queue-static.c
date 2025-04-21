#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
  int *data;
  int start;
  int size;
  int max_size;
} Queue;

int enqueue(Queue *q, int value) {
  if (q == NULL || q->size == q->max_size)
    return 0;

  int end = q->start == -1 ? ++q->start : (q->start + q->size) % q->max_size;
  q->data[end] = value;
  q->size++;
  
  return 1;
}

int dequeue(Queue *q, int *value) {
  if (q == NULL || q->size == 0)
    return 0;

  *value = q->data[q->start];
  q->start = (q->start + 1) % q->max_size;
  q->size--;

  return 1;
}

int peek(Queue *q, int *value) {
  if (q == NULL || q->size == 0)
    return 0;

  *value = q->data[q->start];
  return 1;
}

void print(Queue *q) {
  if (q == NULL || q->size == 0)
    return;

  printf("Start: ");
  for (int i = q->start; i < q->start + q->size; i++) {
    printf("%d ", q->data[i % q->max_size]);
  }
  printf("\n");
}
    

void free_queue(Queue *q) {
  if (q == NULL)
    return;

  free(q->data);
  free(q);
}

Queue *new_queue(int max_size) {
  Queue *q = malloc(sizeof(Queue));
  if (q == NULL) {
    perror("Failed to allocate memory for queue");
    exit(1);
  }

  q->data = malloc(sizeof(int) * max_size);
  if (q->data == NULL) {
    perror("Failed to allocate memory for queue data");
    exit(1);
  }

  q->start = -1;
  q->size = 0;
  q->max_size = max_size;

  return q;
}
