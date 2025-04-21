#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct List {
  Node *next;
  size_t length;
} List;

void add(List *list, int value) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("Failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }

  node->value = value;
  node->next = list->next;
  list->next = node;
  list->length++;
}

Node *search(List *list, int value) {
  if (list == NULL)
    return NULL;

  Node *current = list->next;
  while (current != NULL) {
    if (current->value == value)
      return current;
    current = current->next;
  }

  return NULL;
}

int delete(List *list, int value) {
  if (list == NULL)
    return 0;

  Node *current = list->next, *previous = NULL;
  while (current != NULL) {
    if (current->value == value) {
      if (previous == NULL)
        list->next = current->next;
      else
        previous->next = current->next;
      free(current);
      list->length--;
      return 1;
    }

    previous = current;
    current = current->next;
  }

  return 0;
}

void print(List *list) {
  if (list == NULL)
    return;

  Node *current = list->next;

  printf("HEAD -> ");
  while (current != NULL) {
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("NULL\n");
}

void free_list(List *list) {
  if (list == NULL)
    return;

  while (list->next != NULL) {
    Node *tmp = list->next;
    list->next = tmp->next;
    free(tmp);
  }

  free(list);
}

List *new_list() {
  List *list = malloc(sizeof(List));
  list->next = NULL;
  list->length = 0;
  return list;
}
