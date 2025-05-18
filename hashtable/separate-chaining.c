#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct {
  Node **data;
  size_t size;
  size_t max_size;
} Hashtable;

size_t hash(int value, size_t max_size);
Node *new_node(int value);

// HASHTABLE FUNCTIONS

int hashtable_insert(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL)
    return 0;

  size_t key = hash(value, h->max_size);
  Node *list = h->data[key];

  if (list == NULL) {
    h->data[key] = new_node(value);
    h->size++;
    return 1;
  }

  while (list->value != value && list->next != NULL)
    list = list->next;

  if (list->value == value)
    return 0;

  list->next = new_node(value);
  h->size++;

  return 1;
}

Node *hashtable_search(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL)
    return NULL;

  size_t key = hash(value, h->max_size);
  Node *list = h->data[key];

  while (list != NULL && list->value != value)
    list = list->next;

  return list;
}

int hashtable_remove(Hashtable *h, int value) {
  if (h == NULL || h->data == NULL)
    return 0;

  size_t key = hash(value, h->max_size);
  Node *list = h->data[key];
  Node *prev = NULL;

  if (list == NULL)
    return 0;

  while (list->value != value && list->next != NULL) {
    prev = list;
    list = list->next;
  }

  if (list->value != value)
    return 0;

  if (prev == NULL)
    h->data[key] = list->next;
  else
    prev->next = list->next;

  free(list);
  h->size--;

  return 1;
}

void hashtable_print(Hashtable *h) {
  if (h == NULL || h->data == NULL)
    return;

  for (size_t i = 0; i < h->max_size; i++) {
    printf("[%zu]: ", i);
    Node *list = h->data[i];
    while (list != NULL) {
      printf("%d -> ", list->value);
      list = list->next;
    }
    printf("NULL\n");
  }
}

void hashtable_free(Hashtable *h) {
  if (h == NULL || h->data == NULL)
    return;

  for (size_t i = 0; i < h->max_size; i++) {
    Node *list = h->data[i];
    while (list != NULL) {
      Node *tmp = list;
      list = list->next;
      free(tmp);
    }
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

  h->data = malloc(sizeof(Node *) * max_size);
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

size_t hash(int value, size_t max_size) {
  // change this as needed 
  return value % max_size;
}

Node *new_node(int value) {
  Node *n = malloc(sizeof(Node));
  if (n == NULL) {
    perror("new_node: failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }
  n->value = value;
  n->next = NULL;
  return n;
}
