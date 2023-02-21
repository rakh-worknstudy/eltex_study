#include <stdio.h>
#include <malloc.h>

// #define _ELTEX_DEBUG
/* Тут были стэк операций на случай, если бы делал через польскую
  нотацию и ввод выражений, но от этого пришлось отказаться, т.к.
  мало времени. Но прикола ради это можно задебажить */
#ifdef _ELTEX_DEBUG
#include "eltex_stack.h"
#include "eltex_string.h"
#endif /* _ELTEX_DEBUG */

# ifndef _ELTEX_DEBUG
int eltex_calc_menu(void);
# endif /* _ELTEX_DEBUG */

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
  return 0;
# else
  return eltex_calc_menu();
# endif /* _ELTEX_DEBUG */
}

# ifndef _ELTEX_DEBUG
int eltex_calc_menu(void) {
  long double current;
  long double result = 0.0L;
  //  char *string = NULL;
  char tempmore;
  while (1) {
    printf("Текущее значение: %Lf\n", result);
    printf("1 - Прибавить\n" \
           "2 - Вычесть\n"   \
           "3 - Умножить\n"  \
           "4 - Разделить\n" \
           "5 - Сброс\n"     \
           "0 - Выход\n");
    tempmore = getchar();
    if (tempmore == '0')
      break;
    if (tempmore == '5') {
      result = 0.0L;
      continue;
    }
    if (tempmore > '5' || tempmore < '0')
      continue;
    printf("n = ");
    //  string = eltex_string_get(&string);
    if (scanf("%Lf", &current) == 1) {
      switch (tempmore) {
        case '1':
          result += current;
          break;
        case '2':
          result -= current;
          break;
        case '3':
          result *= current;
          break;
        case '4':
          result /= current;
          break;
        default:
          break;
      }
    } else {
      printf("Плохой ввод\n");
    }
    getchar();
  }
  //  eltex_string_free(&string);
}
# endif /* _ELTEX_DEBUG */