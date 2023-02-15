#include <stdio.h>
#include <malloc.h>
#include "eltex_stack.h"

int eltex_stack_push(eltex_stack_t *stack, char value) {
  if (stack == NULL)
    return -1;
  eltex_elem_t *_elem = eltex_elem_alloc(&_elem);
  _elem->value = value;
  _elem->prev = stack->top;
  stack->top = _elem;
  return 0;
}
char eltex_stack_pop(eltex_stack_t *stack) {
  char _value = 0;
  if (stack != NULL && stack->top != stack->head) {
    _value = stack->top->value;
    eltex_elem_t *_prev = stack->top->prev;
    eltex_elem_free(&stack->top);
    stack->top = _prev;
  }
  return _value;
}

eltex_stack_t *eltex_stack_make(eltex_stack_t **stack) {
  if (stack == NULL)
    return NULL;
  *stack = (eltex_stack_t *)malloc(sizeof(eltex_stack_t));
  eltex_elem_t *_head = eltex_elem_alloc(&_head);
  (*stack)->head = _head;
  (*stack)->top = _head;
  return *stack;
}
int eltex_stack_free(eltex_stack_t **stack) {
  if (stack == NULL)
    return -1;
  while ((*stack)->top != (*stack)->head)
    eltex_stack_pop(*stack);
  eltex_elem_free(&((*stack)->head));
  free(*stack);
  return 0;
}

eltex_elem_t *eltex_elem_alloc(eltex_elem_t **elem) {
  if (elem == NULL)
    return NULL;
  *elem = (eltex_elem_t *)malloc(sizeof(eltex_elem_t));
  return *elem;
}
int eltex_elem_free(eltex_elem_t **elem) {
  free(*elem);
  return 0;
}


