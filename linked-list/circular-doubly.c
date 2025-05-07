#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
  struct Node *prev;
} Node;

void list_insert(Node **list, int value) {
  if (*list == NULL)
    return;

  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("Failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }
  node->value = value;

  if (*list == NULL) {
    node->next = node;
    node->prev = node;
  } else {
    node->next = (*list);
    node->prev = (*list)->prev;
    (*list)->prev->next = node;
    (*list)->prev = node;
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

  Node *current = *list;
  do {
    if (current->value == value) {
      current->prev->next = current->next;
      current->next->prev = current->prev;

      if (current == *list)
        *list = current->next;

      if (current == current->next)
        *list = NULL;

      free(current);
      return 1;
    }
    current = current->next;
  } while (current != *list);

  return 0;
}

void list_print(Node *list) {
  if (list == NULL)
    return;

  Node *current = list;
  do {
    printf("%d <-> ", current->value);
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
