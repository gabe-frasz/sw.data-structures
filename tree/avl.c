#include <stdio.h>
#include <stdlib.h>

typedef struct Tree {
  int value;
  int h;
  struct Tree *left;
  struct Tree *right;
} Tree;

enum PrintType {
  PRE_ORDER,
  IN_ORDER,
  POST_ORDER,
  LEVEL_ORDER,
  BIDIMENSIONAL,
};

int calc_h(Tree *t);
int max(int a, int b);
Tree *new_subtree(int value);
Tree *rotate_left(Tree *t);
Tree *rotate_right(Tree *t);
Tree *rotate_left_right(Tree *t);
Tree *rotate_right_left(Tree *t);
Tree *delete_with_zero_or_one_child(Tree *t);
Tree *delete_with_two_children(Tree *t);
void pre_order_print(struct Tree *t);
void in_order_print(struct Tree *t);
void post_order_print(struct Tree *t);
void level_order_print(struct Tree *t);
void bidimensional_print(struct Tree *t, int b);

// TREE FUNCTIONS

Tree *insert(Tree *t, int value) {
  if (t == NULL)
    return new_subtree(value);

  if (value < t->value)
    t->left = insert(t->left, value);
  if (value > t->value)
    t->right = insert(t->right, value);

  int hl = calc_h(t->left), hr = calc_h(t->right);
  t->h = max(hl, hr) + 1;
  int balance = hl - hr;

  if (balance == 2) {
    if (value < t->left->value)
      return rotate_right(t);
    else
      return rotate_left_right(t);
  }

  if (balance == -2) {
    if (value > t->right->value)
      return rotate_left(t);
    else
      return rotate_right_left(t);
  }

  return t;
}

Tree *search(Tree *t, int value) {
  if (t == NULL)
    return NULL;

  if (value < t->value)
    return search(t->left, value);
  if (value > t->value)
    return search(t->right, value);

  return value == t->value ? t : NULL;
}

Tree *delete(Tree *t, int value) {
  if (t == NULL)
    return NULL;

  if (value < t->value)
    t->left = delete (t->left, value);
  if (value > t->value)
    t->right = delete (t->right, value);

  if (t->value == value) {
    if (t->left == NULL || t->right == NULL)
      t = delete_with_zero_or_one_child(t);
    else
      t = delete_with_two_children(t);
  }

  if (t == NULL)
    return t;

  int hl = calc_h(t->left), hr = calc_h(t->right);
  t->h = max(hl, hr) + 1;
  int balance = hl - hr;

  if (balance == 2) {
    int child_balance = calc_h(t->left->left) - calc_h(t->left->right);

    if (child_balance > 0)
      return rotate_right(t);
    else
      return rotate_left_right(t);
  }

  if (balance == -2) {
    int child_balance = calc_h(t->right->left) - calc_h(t->right->right);

    if (child_balance < 0)
      return rotate_left(t);
    else
      return rotate_right_left(t);
  }

  return t;
}

void print(Tree *t, enum PrintType type) {
  if (t == NULL)
    return;

  switch (type) {
  case PRE_ORDER:
    pre_order_print(t);
    break;
  case IN_ORDER:
    in_order_print(t);
    break;
  case POST_ORDER:
    post_order_print(t);
    break;
  case LEVEL_ORDER:
    level_order_print(t);
    break;
  case BIDIMENSIONAL:
    bidimensional_print(t, 0);
    break;
  }
}

void free_tree(Tree *t) {
  if (t == NULL)
    return;

  free_tree(t->left);
  free_tree(t->right);
  free(t);
}

Tree *new_subtree(int value) {
  Tree *t = malloc(sizeof(Tree));
  if (t == NULL) {
    perror("Failed to allocate memory for tree");
    exit(EXIT_FAILURE);
  }
  t->value = value;
  t->h = 0;
  t->left = NULL;
  t->right = NULL;

  return t;
}

// HELPER FUNCTIONS

int calc_h(Tree *t) { return t == NULL ? -1 : t->h; }

int max(int a, int b) { return a > b ? a : b; }

Tree *rotate_left(Tree *t) {
  Tree *z = t->right;
  t->right = z->left;
  z->left = t;
  t->h = max(calc_h(t->left), calc_h(t->right)) + 1;
  z->h = max(calc_h(z->left), calc_h(z->right)) + 1;
  return z;
}

Tree *rotate_right(Tree *t) {
  Tree *u = t->left;
  t->left = u->right;
  u->right = t;
  t->h = max(calc_h(t->left), calc_h(t->right)) + 1;
  u->h = max(calc_h(u->left), calc_h(u->right)) + 1;
  return u;
}

Tree *rotate_left_right(Tree *t) {
  Tree *u = t->left;
  t->left = rotate_left(u);
  return rotate_right(t);
}

Tree *rotate_right_left(Tree *t) {
  Tree *z = t->right;
  t->right = rotate_right(z);
  return rotate_left(t);
}

Tree *delete_with_zero_or_one_child(Tree *t) {
  if (t->left == NULL) {
    Tree *temp = t->right;
    free(t);
    return temp;
  }

  if (t->right == NULL) {
    Tree *temp = t->left;
    free(t);
    return temp;
  }

  return delete_with_two_children(t);
}

Tree *delete_with_two_children(Tree *t) {
  Tree *p = t;
  Tree *q = t->right;

  while (q->left != NULL) {
    p = q;
    q = q->left;
  }

  t->value = q->value;

  if (p->left == q)
    p->left = delete_with_zero_or_one_child(q);
  else
    p->right = delete_with_zero_or_one_child(q);

  return t;
}

void pre_order_print(Tree *t) {
  if (t == NULL)
    return;
  printf("%d ", t->value);
  pre_order_print(t->left);
  pre_order_print(t->right);
}

void in_order_print(Tree *t) {
  if (t == NULL)
    return;
  in_order_print(t->left);
  printf("%d ", t->value);
  in_order_print(t->right);
}

void post_order_print(Tree *t) {
  if (t == NULL)
    return;
  post_order_print(t->left);
  post_order_print(t->right);
  printf("%d ", t->value);
}

void level_order_print(Tree *t) {
  if (t == NULL)
    return;

  // Queue *q = new_queue();
  // q->enqueue(q, t);

  // while (q->size != 0) {
  //   Tree *t;
  //   q->dequeue(q, &t);
  //   printf("%d ", t->value);
  //   if (t->left != NULL)
  //     q->enqueue(q, t->left);
  //   if (t->right != NULL)
  //     q->enqueue(q, t->right);
  // }
}

void bidimensional_print(Tree *t, int level) {
  if (t == NULL) {
    for (int i = 0; i < level; i++)
      printf("-----");
    printf("NULL\n");
    return;
  }

  bidimensional_print(t->right, level + 1);
  for (int i = 0; i < level; i++)
    printf("-----");
  printf("%d\n", t->value);
  bidimensional_print(t->left, level + 1);
}
