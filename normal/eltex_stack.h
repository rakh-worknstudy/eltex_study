#ifndef ELTEX_STACK_H_
#define ELTEX_STACK_H_

typedef struct Eltex_element {
  char value;
  struct Eltex_element *prev;
} eltex_elem_t;

typedef struct Eltex_stack {
  eltex_elem_t *head;
  eltex_elem_t *top;
} eltex_stack_t;

eltex_stack_t *eltex_stack_make(eltex_stack_t **stack);
int eltex_stack_push(eltex_stack_t *stack, char value);
char eltex_stack_pop(eltex_stack_t *stack);
int eltex_stack_free(eltex_stack_t **stack);

eltex_elem_t *eltex_elem_alloc(eltex_elem_t **elem);
int eltex_elem_free(eltex_elem_t **elem);

#endif /* ELTEX_STACK_H_ */
