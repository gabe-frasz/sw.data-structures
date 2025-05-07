#include <stdio.h>
#include <stdlib.h>

typedef struct Stack {
  int *data;
  int top;
  int size;
  int max_size;
} Stack;

int stack_push(Stack *s, int value) {
  if (s == NULL || s->size == s->max_size)
    return 0;

  s->data[++s->top] = value;
  s->size++;
  return 1;
}

int stack_pop(Stack *s, int *value) {
  if (s == NULL || s->size == 0)
    return 0;

  *value = s->data[s->top--];
  s->size--;
  return 1;
}

int peek(Stack *s, int *value) {
  if (s == NULL || s->size == 0)
    return 0;

  *value = s->data[s->top];
  return 1;
}

void stack_print(Stack *s) {
  if (s == NULL || s->size == 0)
    return;

  printf("Top: ");
  for (int i = s->top; i >= 0; i--)
    printf("%d ", s->data[i]);
  printf("\n");
}

void stack_free(Stack *s) {
  if (s == NULL)
    return;

  free(s->data);
  free(s);
}

Stack *new_stack(int max_size) {
  Stack *s = malloc(sizeof(Stack));
  if (s == NULL) {
    perror("new_stack: failed to allocate memory for stack");
    exit(EXIT_FAILURE);
  }

  s->data = malloc(sizeof(int) * max_size);
  if (s->data == NULL) {
    perror("new_stack: failed to allocate memory for stack data");
    exit(EXIT_FAILURE);
  }
  s->top = -1;
  s->size = 0;
  s->max_size = max_size;

  return s;
}
