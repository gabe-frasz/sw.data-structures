#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int value;
  struct Node *next;
} Node;

typedef struct Stack {
  Node *top;
  size_t size;
} Stack;

void stack_push(Stack *s, int value) {
  Node *node = malloc(sizeof(Node));
  if (node == NULL) {
    perror("stack_push: failed to allocate memory for node");
    exit(EXIT_FAILURE);
  }

  node->value = value;
  node->next = s->top;

  s->top = node;
  s->size++;
}

int stack_pop(Stack *s, int *value) {
  if (s == NULL || s->size == 0)
    return 0;

  Node *node = s->top;
  *value = node->value;
  s->top = node->next;
  s->size--;

  free(node);
  return 1;
}

int stack_peek(Stack *s, int *value) {
  if (s == NULL || s->size == 0)
    return 0;

  *value = s->top->value;
  return 1;
}

void stack_print(Stack *s) {
  if (s == NULL || s->size == 0)
    return;

  Node *node = s->top;

  printf("Top: ");
  while (node != NULL) {
    printf("%d ", node->value);
    node = node->next;
  }
  printf("\n");
}

void stack_free(Stack *s) {
  if (s == NULL)
    return;

  while (s->top != NULL) {
    Node *tmp = s->top;
    s->top = tmp->next;
    free(tmp);
  }
  free(s);
}

Stack *new_stack() {
  Stack *s = malloc(sizeof(Stack));
  if (s == NULL) {
    perror("new_stack: failed to allocate memory for stack");
    exit(EXIT_FAILURE);
  }
  s->top = NULL;
  s->size = 0;

  return s;
}
