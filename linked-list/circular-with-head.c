#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
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

  int is_empty = (*list)->head.next == NULL || (*list)->length == 0;
  node->next = is_empty ? &(*list)->head : (*list)->head.next;

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

int list_remove(List *list, int value) {
  if (list == NULL || list->length == 0)
    return 0;

  Node *current = list->head.next, *previous = NULL;
  while (current != &list->head) {
    if (current->value == value)
      break;
    previous = current;
    current = current->next;
  }

  int not_found = current == &list->head && current->value != value;
  int has_only_one = current == list->head.next && current->next == &list->head;
  int is_first = current == list->head.next && previous == NULL;

  if (not_found)
    return 0;

  if (has_only_one)
    list->head.next = NULL;
  else if (is_first)
    list->head.next = current->next;
  else
    previous->next = current->next;

  list->length--;
  free(current);
  return 1;
}

void list_print(const List *list) {
  if (list == NULL || list->length == 0)
    return;

  Node *current = list->head.next;

  printf("HEAD -> ");
  while (current != &list->head) {
    printf("%d -> ", current->value);
    current = current->next;
  }
  printf("LOOP\n");
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
  list->head.next = NULL;
  list->length = 0;

  return list;
}
