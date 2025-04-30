#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *data;
  size_t size;
  size_t max_size;
} Heap;

int parent(int i);
int left(int i);
int right(int i);
void up(Heap *h, int i);
void down(Heap *h, int i);
void print_recursive(Heap *h, int i, int level);

// HEAP FUNCTIONS

int insert(Heap *h, int value) {
  if (h == NULL || h->size == h->max_size)
    return 0;

  h->data[++h->size] = value;
  up(h, h->size);

  return 1;
}

int remove_max(Heap *h, int *value) {
  if (h == NULL || h->size == 0)
    return 0;

  *value = h->data[1];
  h->data[1] = h->data[h->size--];
  down(h, 1);

  return 1;
}

void print(Heap *h) {
  if (h == NULL || h->size == 0)
    return;

  print_recursive(h, 1, 0);
}

void free_heap_owned(Heap *h) {
  if (h == NULL)
    return;

  free(h->data);
  free(h);
}

void free_heap_view(Heap *h) {
  if (h == NULL)
    return;

  free(h);
}

Heap *new_heap(size_t max_size) {
  Heap *h = malloc(sizeof(Heap));
  if (h == NULL) {
    perror("Failed to allocate memory for heap");
    exit(EXIT_FAILURE);
  }
  h->data = malloc(sizeof(int) * (max_size + 1));
  h->size = 0;
  h->max_size = max_size;
  return h;
}

// HELPER FUNCTIONS

int parent(int i) { return i / 2; }

int left(int i) { return 2 * i; }

int right(int i) { return 2 * i + 1; }

void up(Heap *h, int i) {
  int p = parent(i);
  if (p > 0 && h->data[i] > h->data[p]) {
    int tmp = h->data[i];
    h->data[i] = h->data[p];
    h->data[p] = tmp;
    up(h, p);
  }
}

void down(Heap *h, int i) {
  int l = left(i);
  int r = right(i);
  int biggest = i;

  if (l <= h->size && h->data[l] > h->data[biggest])
    biggest = l;
  if (r <= h->size && h->data[r] > h->data[biggest])
    biggest = r;

  if (biggest != i) {
    int tmp = h->data[i];
    h->data[i] = h->data[biggest];
    h->data[biggest] = tmp;
    down(h, biggest);
  }
}

void print_recursive(Heap *h, int i, int level) {
  if (i > h->size) {
    for (int j = 0; j < level; j++)
      printf("-----");
    printf("NULL\n");
    return;
  }

  print_recursive(h, right(i), level + 1);
  for (int j = 0; j < level; j++)
    printf("-----");
  printf("%d\n", h->data[i]);
  print_recursive(h, left(i), level + 1);
}

Heap *heapify_up(int **data, size_t size, size_t max_size) {
  if (data == NULL || *data == NULL || size == 0 || max_size < size)
    return NULL;

  Heap *h = malloc(sizeof(Heap));
  int *tmp = realloc(*data, sizeof(int) * (max_size + 1));
  if (h == NULL || tmp == NULL)
    return NULL;

  *data = tmp;
  (*data)[size] = (*data)[0];

  h->data = *data;
  h->size = size;
  h->max_size = max_size;

  for (int i = size / 2; i >= 1; i--)
    down(h, i);

  return h;
}

Heap *heapify_down(int **data, size_t size, size_t max_size) {
  if (data == NULL || *data == NULL || size == 0 || max_size < size)
    return NULL;

  Heap *h = malloc(sizeof(Heap));
  int *tmp = realloc(*data, sizeof(int) * (max_size + 1));
  if (h == NULL || tmp == NULL)
    return NULL;

  *data = tmp;
  (*data)[size] = (*data)[0];

  h->data = *data;
  h->size = size;
  h->max_size = max_size;

  for (int i = 2; i <= size; i++)
    up(h, i);

  return h;
}

int heap_sort(int **data, size_t data_size) {
  Heap *h = heapify_down(data, data_size, data_size);
  if (h == NULL)
    return 0;

  for (int i = data_size; i > 1; i--) {
    int tmp = h->data[1];
    h->data[1] = h->data[i];
    h->data[i] = tmp;
    h->size--;
    down(h, 1);
  }

  free_heap_view(h);
  return 1;
}
