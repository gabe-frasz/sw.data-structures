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
    exit(EXIT_FAILURE);
  }

  node->value = value;
  node->next = *list;
  *list = node;
}

Node *list_search(Node *list, int value) {
  if (list == NULL)
    return NULL;

  while (list != NULL) {
    if (list->value == value)
      return list;
    list = list->next;
  }

  return NULL;
}

int list_remove(Node **list, int value) {
  if (*list == NULL)
    return 0;

  Node *current = *list;
  Node *previous = NULL;

  while (current != NULL) {
    if (current->value == value) {
      if (previous == NULL)
        *list = current->next;
      else
        previous->next = current->next;
      free(current);
      return 1;
    }

    previous = current;
    current = current->next;
  }

  return 0;
}

void list_print(Node *list) {
  if (list == NULL)
    return;

  while (list != NULL) {
    printf("%d -> ", list->value);
    list = list->next;
  }
  printf("NULL\n");
}

void list_free(Node *list) {
  if (list == NULL)
    return;

  while (list != NULL) {
    Node *tmp = list;
    list = list->next;
    free(tmp);
  }
}
