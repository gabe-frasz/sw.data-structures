#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int **data;
  size_t size;
  size_t max_size;
} Hashtable;

size_t hash(int value, size_t max_size, int k);

// HASHTABLE FUNCTIONS

int hashtable_insert(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL || h->size == h->max_size)
    return 0;

  size_t key;

  for (int k = 0; k < h->max_size; k++) {
    key = hash(value, h->max_size, k);

    if (h->data[key] != NULL) {
      // do not allow duplicate values
      if (*(h->data[key]) == value)
        return 0;
      else
        continue;
    }

    h->data[key] = malloc(sizeof(int));
    if (h->data[key] == NULL) {
      perror("hashtable_insert: failed to allocate memory for hashtable value");
      exit(EXIT_FAILURE);
    }

    *(h->data[key]) = value;
    h->size++;

    return 1;
  }

  return 0;
}

int *hashtable_search(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL || h->size == 0)
    return NULL;

  size_t key;

  for (int k = 0; k < h->max_size; k++) {
    key = hash(value, h->max_size, k);

    if (h->data[key] != NULL && *(h->data[key]) == value)
      return h->data[key];
  }

  return NULL;
}

int hashtable_remove(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL || h->size == 0)
    return 0;

  size_t key;

  for (int k = 0; k < h->max_size; k++) {
    key = hash(value, h->max_size, k);

    if (h->data[key] != NULL && *(h->data[key]) == value) {
      free(h->data[key]);
      h->data[key] = NULL;
      h->size--;
      return 1;
    }
  }

  return 0;
}

void hashtable_print(Hashtable *h) {
  if (h == NULL || h->data == NULL)
    return;

  for (size_t i = 0; i < h->max_size; i++) {
    printf("[%zu]: ", i);
    if (h->data[i] != NULL)
      printf("%d\n", *(h->data[i]));
    else
      printf("NULL\n");
  }
}

void hashtable_free(Hashtable *h) {
  if (h == NULL || h->data == NULL)
    return;

  for (size_t i = 0; i < h->max_size; i++) {
    if (h->data[i] != NULL)
      free(h->data[i]);
  }

  free(h->data);
  free(h);
}

Hashtable *new_hashtable(size_t max_size) {
  Hashtable *h = malloc(sizeof(Hashtable));
  if (h == NULL) {
    perror("new_hashtable: failed to allocate memory for hashtable");
    exit(EXIT_FAILURE);
  }

  h->data = malloc(sizeof(int *) * max_size);
  if (h->data == NULL) {
    perror("new_hashtable: failed to allocate memory for hashtable data");
    exit(EXIT_FAILURE);
  }
  h->size = 0;
  h->max_size = max_size;

  for (size_t i = 0; i < max_size; i++)
    h->data[i] = NULL;

  return h;
}

// HELPER FUNCTIONS

size_t _hash(int value, size_t max_size) { return value % max_size; }

size_t hash(int value, size_t max_size, int k) {
  return (_hash(value, max_size) + k) % max_size; // linear probing
}
