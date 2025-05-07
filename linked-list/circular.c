#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

void list_insert(Node **list, int value) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("Failed to allocate memory for node");
    exit(1);
  }
  node->value = value;

  if (*list == NULL) {
    node->next = node;
  } else {
    Node *current = *list;
    while (current->next != *list)
      current = current->next;
    current->next = node;
    node->next = *list;
  }

  *list = node;
}

Node *list_search(Node *list, int value) {
  if (list == NULL)
    return NULL;

  Node *current = list;

  do {
    if (current->value == value)
      return current;
    current = current->next;
  } while (current != list);

  return NULL;
}

int list_remove(Node **list, int value) {
  if (*list == NULL)
    return 0;

  Node *current = *list, *previous = NULL;
  do {
    if (current->value == value)
      break;
    previous = current;
    current = current->next;
  } while (current != *list);

  int not_found = current == *list && current->value != value;
  int has_only_one = current == *list && current->next == *list;
  int is_first = current == *list && previous == NULL;

  if (not_found)
    return 0;

  if (has_only_one) {
    *list = NULL;
  } else if (is_first) {
    Node *tmp = current;
    while (tmp->next != *list)
      tmp = tmp->next;
    tmp->next = (*list)->next;
    *list = (*list)->next;
  } else {
    previous->next = current->next;
  }

  free(current);
  return 1;
}

void list_print(Node *list) {
  if (list == NULL)
    return;

  Node *current = list;
  do {
    printf("%d -> ", current->value);
    current = current->next;
  } while (current != list);

  printf("LOOP\n");
}

void list_free(Node *list) {
  if (list == NULL)
    return;

  Node *current = list;
  do {
    Node *tmp = current;
    current = current->next;
    free(tmp);
  } while (current != list);
}
