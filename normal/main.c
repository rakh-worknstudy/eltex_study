#include <stdio.h>
#include <malloc.h>
#include "eltex_string.h"
#include "eltex_stack.h"

#define _ELTEX_DEBUG

int main(void) {
# ifdef _ELTEX_DEBUG
  char *string;
  printf("%s", "INPUT STRING: ");
  string = eltex_string_get(&string);
  printf("%s", "STRING CREATED\n");
  eltex_stack_t *stack;
  stack = eltex_stack_make(&stack);
  printf("%s", "PUSHING STRING TO STACK\n");
  for (char *ptr = string; *ptr; ++ptr) {
    eltex_stack_push(stack, *ptr);
    printf("%c", *ptr);
  }
  printf("%s", "\nOK\n");
  printf("%s%c\n", "STACK TOP VALUE: ", stack->top->value);
  char buffer;
  printf("%s", "STACK POP: ");
  while ((buffer = eltex_stack_pop(stack)))
    printf("%c", buffer);
  printf("\n%s", "FREEING STACK:");
  eltex_stack_free(&stack);
  printf("%s", " DONE\nFREEING STRING:");
  eltex_string_free(&string);
  printf("%s", " DONE\n");
# elif
  
# endif
  return 0;
}
