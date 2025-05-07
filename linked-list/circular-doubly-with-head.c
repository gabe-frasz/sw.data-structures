#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
  struct Node *prev;
} Node;

typedef struct List {
  Node head;
  size_t length;
} List;

void list_insert(List **list, int value) {
  if (list == NULL)
    return;

  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("Failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }
  node->value = value;

  if ((*list)->length == 0)
    (*list)->head.prev = node;

  node->next = (*list)->head.next;
  node->prev = &(*list)->head;

  (*list)->head.next->prev = node;
  (*list)->head.next = node;
  (*list)->length++;
}

Node *list_search(const List *list, int value) {
  if (list == NULL || list->length == 0)
    return NULL;

  Node *current = list->head.next;
  while (current != &list->head) {
    if (current->value == value)
      return current;
    current = current->next;
  }

  return NULL;
}

int list_remove(List **list, int value) {
  if (list == NULL || (*list)->length == 0)
    return 0;

  Node *current = (*list)->head.next;
  while (current != &(*list)->head) {
    if (current->value == value) {
      current->prev->next = current->next;
      current->next->prev = current->prev;

      if (current == (*list)->head.next)
        (*list)->head.next = current->next;

      if (current == (*list)->head.prev)
        (*list)->head.prev = current->prev;

      (*list)->length--;
      free(current);
      return 1;
    }
    current = current->next;
  }

  return 0;
}

void list_print(const List *list) {
  if (list == NULL || list->length == 0)
    return;

  Node *current = list->head.next;
  printf("HEAD <-> ");
  while (current != &list->head) {
    printf("%d <-> ", current->value);
    current = current->next;
  }
  printf("HEAD\n");
}

void list_free(List *list) {
  if (list == NULL || list->length == 0)
    return;

  Node *current = list->head.next;
  while (current != &list->head) {
    Node *tmp = current;
    current = current->next;
    free(tmp);
  }
  free(list);
}

List *new_list() {
  List *list = malloc(sizeof(List));
  if (list == NULL) {
    perror("Failed to allocate memory for list");
    exit(EXIT_FAILURE);
  }
  list->head.next = &list->head;
  list->head.prev = &list->head;
  list->length = 0;

  return list;
}
